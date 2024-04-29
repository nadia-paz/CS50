import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # TODO
        self.positive = set()
        self.negative = set()
        
        with open(positives) as lines:
            for line in lines:
                if not line.startswith(';'):
                    self.positive.add(line.strip())
                    # self.positive.add(line.rstrip('.! \n'))
        
        nfile = open(negatives, "r")
        for line in nfile:
            if not line.startswith(';'):
                self.negative.add(line.strip())
        nfile.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # TODO
        score = 0
        tokenizer = nltk.tokenize.TweetTokenizer()
        # tokens = self.tokenizer.tokenize(text)
        tokens = tokenizer.tokenize(text)
        
        for word in tokens:
            if word.lower() in self.positive:
                score += 1
            elif word.lower() in self.negative:
                score -= 1
        
        
        return score
