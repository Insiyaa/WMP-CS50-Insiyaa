import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positive = set()
        self.negative = set()

        with open(positives) as lines:
            for line in lines:
                if not line.startswith(';'):
                    self.positive.add(line.rstrip('\n'))
            lines.close()

        with open(negatives) as lines:
            for line in lines:
                if not line.startswith(';'):
                    self.negative.add(line.rstrip('\n'))
            lines.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        score = 0
        for e in tokens:
            if e in self.positive:
                score += 1
            elif e in self.negative:
                score -= 1

        return score

