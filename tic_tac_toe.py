
class TicTacToe:

    def __init__(self):
        self.current_player = "X"

    def get_player(self):
        return self.current_player

    def set_player(self, player):
        self.current_player = player
