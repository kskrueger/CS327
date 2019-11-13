//
// Created by Karter Krueger on 11/12/19.
//

#include "Checkers.h"

int main(int argc, char* argv[]) {
    read_args(argc, argv);
    scan_input();
    int count_of_moves = count_moves();
    if (n_moves == -1 || n_moves > count_of_moves) n_moves = count_of_moves;

    //fprintf(stdout, "Input file has %d moves\n", count_of_moves);
    //fprintf(stdout, "Depth of %d moves\n", d_depth);
    //fprintf(stdout, "Processing %d moves\n", n_moves);
    //print_board_full(stdout, ROWS, COLS, board);
    make_moves();
    //if (!illegal_move) fprintf(stdout, "All moves are legal\n");

    //print_board_full(stdout, ROWS, COLS, board);

    //move b_moves = NULL;
    //find_moves(&b_moves);
    start_recurse(board, 1);

    //print_board_full(stdout, ROWS, COLS, board);
    /*Scoring
     * #r = number of red pawns, #B = number of black kings
     * Score for red is the negative of the score for black
     * RED SCORE:
        * -99 if red has no moves
        * 99 if black has no moves
        * (#r + 2 * #R) - (#b + 2 * #B) otherwise
     * BLACK SCORE:
        * -99 if black has no moves
        * 99 if red has no moves
        * (#b + 2 * #B) - (#r + 2 * #R) otherwise
     */


    /* Lookahead
     * No Lookahead (D = 0)
        * Red turn, two moves available (d4-b6, d4-e5)
        * With D=0, scored as:
            * d4-b6: after move there is one red pawn and one black pawn, giving score of 0 (black pawn is jumped)
            * d4-e5: after move there is one red pawn and two black pawns, giving score of -1
     * With lookahead
        * Determine all moves available for the opponent player, and score them with a lookahead of D-1
        * If no moves available for the opponent, then move m has a score of 99
        * Otherwise, assume opponent chooses a move that will maximize her score (minimize current player score)
        * Score for move m is then the best outcome score the current player can expect
        * EX: (D = 1)
            * Red can go d4->d6 or d4->e5
            * Taking d4->b6 jumps black and removes a pawn
            * Now it is black's turn, with an exploration depth of 0
            * which can go c7->a5 for a score of 99 (jumps red) or c7->d6 for score 0 (blank)
            * The final score is black 99, red -99
            *
            * If red instead took d4->e5
            * Black turn with D=0
            * Moves: c5->b4, c5->d4, c7->b6, c7->d7
            * Each move will result with score 1 for black
            * Thus, the score for red move d4->e5 is -1
     */

    /* Output:
     * If no moves, output:
        * Player (player <red or black>) has no moves available
     * Otherwise, all current moves for current player should be displayed in alphabetical order (easier to compare units)
     * If verbose flag set, all considered moves at each depth should be displayed (in alphabetical order)
     *
     * Non-Verbose output (list of moves with scores for current player)
     * (move) for (player): score (value)
     *
     * Verbose output
     * If depth > 0, display following when considering a move:
        * (indent)? (move) for (player):
        * (indent) number of spaces
        * List moves considered for other player
        * Indent extra for next line, listing moves considered
     */

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-e")) {
            fileE(argv, &i);
        } else if (!strcmp(argv[i], "-h")) {
            fileH(argv, &i);
        }
    }

    return 0;
}
