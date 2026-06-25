#include <bits/stdc++.h>
#include "chess.hpp"
using namespace std;

class Solver {
    chess::Board board;
    unordered_map<string, chess::Move> strat;

public:
    Solver(const string &fen) : board(chess::Board::fromFen(fen)) {}

    chess::Movelist solve() {
        alpha_beta();
        chess::Movelist ans;
        auto cur = board;

        for (auto it = strat.find(cur.getFen()); it != strat.end();
            it = strat.find(cur.getFen())) {
            ans.add(it->second);
            cout << chess::uci::moveToSan(cur, it->second) << '\n';
            cur.makeMove(it->second);
}
        return ans;
    }

    int alpha_beta(bool player = true, int depth = 0,
                   int alpha = INT_MIN, int beta = INT_MAX) {

        if (!chess::movegen::anylegalmoves(board))
            return board.inCheck() ? (player ? -1 : 1) : 0;

        if (depth == 4 || board.isHalfMoveDraw() ||
            board.isInsufficientMaterial() || board.isRepetition())
            return 0;

        chess::Movelist moves;
        chess::movegen::legalmoves(moves, board);

        chess::Move best;
        int util = player ? -2 : 2;

        for (auto move : moves) {
            board.makeMove(move);
            int val = alpha_beta(!player, depth + 1, alpha, beta);
            board.unmakeMove(move);

            if ((player && val > util) || (!player && val < util))
                best = move, util = val;

            player ? alpha = max(alpha, val) : beta = min(beta, val);
            if (alpha >= beta) break;
        }

        strat[board.getFen()] = best;
        return util;
    }
};

bool is_fen(const string &s) {
    int slashes = 0;
    for (char c : s) {
        if (c == '/') {
            slashes++;
        }
    }
    return slashes == 7;
}

int main() {
    string line;
    while (getline(cin, line)) {
        if (is_fen(line)) {
            cout << "Solving: " << line << "\n";
            Solver(line).solve();
            cout << "----------------------------------------\n";
        }
    }
}