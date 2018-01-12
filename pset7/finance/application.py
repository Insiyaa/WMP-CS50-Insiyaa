from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    # array of stocks_dicts
    stocks = []

    # getting total number of shares
    price = 0
    rows = db.execute("SELECT symbol, SUM(number_of_shares) FROM history WHERE user_id = :id GROUP BY symbol;", id = session['user_id'])
    if not rows:
        print(rows)
        return render_template("index.html",cash = usd(1000), worth = usd(1000))

    for row in rows:
        # dictionary with stock's number, symbol, curr_value and total
        stock_dict = {}
        # don't display if total equals to zero
        if row['SUM(number_of_shares)'] == 0:
            continue
        stock_dict['number'] = row['SUM(number_of_shares)']
        stock_dict['symbol'] = row['symbol']
        print(row['symbol'])
        look = lookup(row['symbol'])
        stock_dict['value'] = look['price']
        stock_dict['total'] = stock_dict['number'] * stock_dict['value']
        price += stock_dict['total']
        stocks.append(stock_dict)

    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session['user_id'])[0]['cash']
    worth = cash + price
    return render_template("index.html", stocks = stocks, cash = usd(cash), worth = usd(worth))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":

        # ensure username was submitted
        symbol = request.form.get("symbol")
        num = request.form.get("num")

        if not symbol:
            return apology("must provide stock symbol")

        # ensure password was submitted
        elif not num:
            return apology("must provide number of shares")

        stocks = lookup(symbol)
        amount = stocks['price'] * int(num)

        query = db.execute("SELECT cash FROM users WHERE id = :id", id = session['user_id'])
        balance = query[0]["cash"]

        # check if affordable
        if balance < amount:
            return apology("not affordable")
        else:
            db.execute("UPDATE users SET cash = cash - :amount where id = :id", id = session['user_id'], amount = amount)
            db.execute("INSERT INTO history(user_id, symbol, price, number_of_shares, action, date_time) VALUES(:id, :sym, :price, :num, 'BUY', CURRENT_TIMESTAMP)",
            id = session['user_id'], sym = symbol, price = float(stocks['price']), num = int(num))
            flash(num + " shares of " + symbol.upper() + " bought!")

        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    rows = db.execute("SELECT * FROM history WHERE user_id = :id",id = session['user_id'])
    return render_template("history.html", rows = rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]
        flash("You're logged in!")

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    flash("Logged out successfully.")
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        """Get stock quote."""
        symbol = request.form.get("stockSymbol")
        if not symbol:
            return apology("provide stock symbol")
        else:
            quote = lookup(symbol)
        if not quote:
            return apology("invalid stock symbol")
        # render stock.html and print stuffs
        return render_template("stock.html", name = quote["name"], price = quote["price"], symbol = quote["symbol"])
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    if request.method == "POST":

        # ensure username was submitted
        if request.form.get("username") == '':
            return apology("must provide username")

        # ensure password was submitted
        elif request.form.get("password") == '':
            return apology("must provide password")

        # compares both passwords
        elif not request.form.get("password") == request.form.get("passwordConf"):
            return apology("password doesn't matches")
        else:
            hash = pwd_context.hash(request.form.get("password"))

        #If an INSERT, then execute returns the value of the newly inserted row’s primary key.

        result = db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)",
        username = request.form.get("username"), hash = hash)

        if not result:
            return apology("username already exists")

        # remember which user has logged in
        session["user_id"] = result
        flash("You've been registered!")
        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        # take users input of shrae symbol
        symbol = request.form.get("symbol")
        if symbol == 'none':
            return apology("Please enter stock symbol")
        # valid user's input of number of shares
        try:
            num = int(request.form.get("num"))
            if num < 0:
                return apology("shares must be positive number")
        except:
            return apology("shares must be positive number")
        # collect total number of shares of that stock
        share = db.execute("SELECT symbol, SUM(number_of_shares) FROM history WHERE user_id = :id AND symbol = :symbol GROUP BY symbol;",
        id = session['user_id'], symbol = symbol)
        # check if enough shares available
        avail = share[0]['SUM(number_of_shares)']
        if avail < num:
            return apology("Not enough shares")
        # insert the query in history
        print(symbol)
        stocks = lookup(symbol)
        price = float(stocks['price'])
        print(price)
        db.execute("INSERT INTO history(user_id, symbol, price, number_of_shares, action, date_time) VALUES(:id, :sym, :price, :num, 'SELL', CURRENT_TIMESTAMP)",
            id = session['user_id'], sym = symbol, price = price, num = -int(num))
        # update user's cash
        amount = stocks['price'] * num
        db.execute("UPDATE users SET cash = cash + :amount where id = :id", id = session['user_id'], amount = amount)
        flash("{} shares of {} sold at the price of {}.".format(num, symbol.upper(), price))
        return redirect(url_for("index"))

    else:
        share = set()
        dicts =  db.execute("SELECT symbol, SUM(number_of_shares) FROM history WHERE user_id = :id GROUP BY symbol;", id = session['user_id'])
        for x in dicts:
            if x['SUM(number_of_shares)'] != 0:
                share.add(x['symbol'])
        shares = list(share)
        print(shares)
        return render_template("sell.html", shares = shares)

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change password"""
    if request.method == "POST":
        data = db.execute("SELECT * FROM users WHERE id = :id",id = session['user_id'])
        username = data[0]['username']
        # check if user knows it's own username as validation
        if not username == request.form.get("username"):
            apology("Username invalid")
        password = request.form.get("password")
        if not password:
            return apology("enter password")
        if request.form.get("passwordConf") != password:
            return apology("Password does not matches")
        hash = pwd_context.hash(password)
        db.execute("UPDATE users SET hash = :hash where id = :id", id = session['user_id'], hash =  hash)
        flash("Password changed successfully!")
        return redirect(url_for("index"))
    else:
        return render_template("change.html")


