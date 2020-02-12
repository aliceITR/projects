package com.airblair.chikuchakutoe;

import java.util.Random;

public class GameBoard {

    private static final Random RANDOM = new Random();
    private char[] sections;
    private char currentPlayer;
    private boolean ended;

    public GameBoard() {
        sections = new char[9];
        newGame();
    }

    public boolean isEnded() {
        return ended;
    }

    public char play(int x, int y) {
        if (!ended  &&  sections[3 * y + x] == ' ') {
            sections[3 * y + x] = currentPlayer;
            changePlayer();
        }
        return checkIfEnd();
    }

    public void changePlayer() {
        if ( currentPlayer == 'X')
        {
            currentPlayer = 'O';
        }
        else
        {
            currentPlayer = 'X';
        }
    }

    public char getSection(int x, int y) {
        return sections[3 * y + x];
    }

    public void newGame() {
        for (int i = 0; i  < sections.length; i++) {
            sections[i] = ' ';
        }
        currentPlayer = 'X';
        ended = false;
    }

    public char checkIfEnd() {
        for (int i = 0; i < 3; i++) {
            if (getSection(i, 0) != ' ' &&
                    getSection(i, 0) == getSection(i, 1)  &&
                    getSection(i, 1) == getSection(i, 2)) {
                ended = true;
                return getSection(i, 0);
            }

            if (getSection(0, i) != ' ' &&
                    getSection(0, i) == getSection(1, i)  &&
                    getSection(1, i) == getSection(2, i)) {
                ended = true;
                return getSection(0, i);
            }
        }

        if (getSection(0, 0) != ' '  &&
                getSection(0, 0) == getSection(1, 1)  &&
                getSection(1, 1) == getSection(2, 2)) {
            ended = true;
            return getSection(0, 0);
        }

        if (getSection(2, 0) != ' '  &&
                getSection(2, 0) == getSection(1, 1)  &&
                getSection(1, 1) == getSection(0, 2)) {
            ended = true;
            return getSection(2, 0);
        }

        for (int i = 0; i < 9; i++) {
            if (sections[i] == ' ')
                return ' ';
        }

        return 'T';
    }

    //this is a small AI but it is only Random num generator
    public char compute() {
        if (!ended) {
            int position = -1;

            do {
                position = RANDOM.nextInt(9);
            } while (sections[position] != ' ');

            sections[position] = currentPlayer;
            changePlayer();
        }

        return checkIfEnd();
    }

}





