/**
 * G2312
 * Niculae Stefan 4 1
 */


package com.company;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;


public class Main
{
    public static void main (String[] args)
    {
        /*
            Example input:
            6 4
            2
            0 0 0 0
            9 2 1 3
            9 2 5 1
            9 1 4 1
            0 0 0 0
            0 0 0 0
         */

        OptimalExplorer explorer = new OptimalExplorer ("input.txt");
        explorer.solve ();
        explorer.traceback ();
    }
}


class OptimalExplorer
{
    int m, n;           // number of rows and columns
    int c;              // starting column
    int[][] cost;       // map altitudes

    int[][] effort;     // effort[i, j] = minimum effort required to reach square (i, j)
    int[][] prev;       // prev[i, j]   = previous square (left, up or right)

    final int INFINITY  = Integer.MAX_VALUE;
    final int LEFT   = -1,
              ABOVE  =  0,
              RIGHT  = +1,
              NO_WAY =  2;

    static class Point
    {
        int x, y;
        public Point (int x, int y) { this.x = x; this.y = y; }
        @Override
        public String toString () { return "(" + x + ", " + y + ")"; }
    }

    public OptimalExplorer (String inputPath)
    {
        readInput (inputPath);
    }

    void readInput (String path)
    {
        try {
            Scanner inFile = new Scanner (new File (path));

            m = inFile.nextInt ();
            n = inFile.nextInt ();
            c = inFile.nextInt ();

            cost = new int[m][n];
            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    cost[i][j] = inFile.nextInt ();


            inFile.close ();
        } catch (FileNotFoundException e) {
            e.printStackTrace ();
        }
    }

    void solve ()
    {
        // Initializations
        effort = new int[m][n];
        prev   = new int[m][n];

        // Explorer can only start on the column C of the first row
        cost[0][c] = 0;

        for (int i = 0; i < m; i++) {
            // Before computing, effort to get anywhere is infinite...
            Arrays.fill (effort[i], INFINITY);
            // ...and the previous direction is unknown
            Arrays.fill (prev[i], NO_WAY);
        }
        // The explorer starts here, so the effort to get here is zero
        effort[0][c] = 0;


        // Start on the second row
        for (int i = 1; i < m; i++)
            // On the square [i, j] we can get from either above, above left or above right
            for (int j = 0; j < n; j++) {

                // The explorer can only go in a 'triangle' each row
                // so there is no need to compute effort for squares he can't reach
                if (Math.abs (j - c) > i)
                    continue;

                // Three values for the three ways we can get on this square
                int aboveEff, leftEff, rightEff;

                // Make sure we don't go out of bounds (j < 0 or j >= n)
                aboveEff = effort[i - 1][j];
                leftEff  = j > 0   ? effort[i - 1][j - 1] : INFINITY;
                rightEff = j < n-1 ? effort[i - 1][j + 1] : INFINITY;

                // The explorer will reach this square coming from the one with minimum effort
                int minEff = min (aboveEff, leftEff, rightEff);

                // We have to spend the effort to get here anyway
                // but maybe we spent more earlier
                effort[i][j] = Math.max (cost[i][j], minEff);

                // Remember where the explorer came from so we can trace back the path
                if (minEff == aboveEff) prev[i][j] = ABOVE;
                if (minEff ==  leftEff) prev[i][j] = LEFT;
                if (minEff == rightEff) prev[i][j] = RIGHT;


                System.out.format ("i = %d, j = %d, cost = %d\nL = %d, A = %d, R = %d, min = %d\n=> dir = %d\n", i, j, cost[i][j], leftEff, aboveEff, rightEff, minEff, prev[i][j]);
                printConfig (i, j);
            }




    }

    public void traceback ()
    {
        int minEff = INFINITY;
        int minIndex = -1;
        // Look on the last line and get the minimum
        for (int i = 0; i < n; i++)
            if (effort[m - 1][i] < minEff) {
                minEff = effort[m - 1][i];
                minIndex = i;
            }


        List<Point> path = new ArrayList<Point> (m);
        int col = minIndex;

        // Start from the bottom row and the column of the minimum effort
        // and trace back the path upwards
        for (int row = m-1; row >= 0; row--) {
            path.add (new Point (row, col));
            col += prev[row][col];
        }
        // Reverse the list so it starts from the top and goes downwards
        Collections.reverse (path);

        System.out.format ("Path for minimum effort (%d) = %s\n", minEff, path);
    }

    void printConfig (int row, int col)
    {
        System.out.println ("\n     Cost:          Effort:          Prev:");
        printHorizontalSeparator (3);

        for (int i = 0; i < m; i++) {
            for (int k = 0; k < 3; k++) {
                System.out.print ("|");
                for (int j = 0; j < n; j++) {

                    if ((k == 1 || k == 2) && i == row && j == col)
                        System.out.print ("[");
                    else
                        System.out.print (" ");

                    // Cost column
                    if (k == 0)
                        if (i == 0 && j != c) System.out.print (" ");
                        else System.out.print (cost[i][j]);

                    // Effort column
                    if (k == 1)
                        if (effort[i][j] == INFINITY) System.out.print (" ");
                        else System.out.print (effort[i][j]);

                    // Prev column
                    if (k == 2)
                        switch (prev[i][j]) {
                            case LEFT:
                                System.out.print ("<");
                                break;
                            case ABOVE:
                                System.out.print ("^");
                                break;
                            case RIGHT:
                                System.out.print (">");
                                break;
                            case NO_WAY:
                                System.out.print (" ");
                                break;
                    }


                    if ((k == 1 || k == 2) && i == row && j == col)
                        System.out.print ("]");
                    else
                        System.out.print (" ");
                } // j (column)
                System.out.print ("|  ");
            } // k (matrix nr)
            System.out.println ();
        } // i (row)

        printHorizontalSeparator (3);
        System.out.println ("\n\n\n");
    }

    void printHorizontalSeparator (int count)
    {
        System.out.print (" ");
        for (int k = 0; k < count; k++) {
            for (int i = 0; i < 3 * n; i++)
                System.out.print ("-");
            System.out.print ("    ");
        }

        System.out.println ();


    }

    int min (int a, int b, int c)
    {
        int minAB = Math.min (a, b);
        return Math.min (minAB, c);
    }

}
