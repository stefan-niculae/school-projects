/**
 * Stefan Niculae & Hung Trinh, grupa 231
 * An 2 Sem I, dec 2014
 * <p>
 * Proiect laborator Geometrie Computationala
 * 11. Pozitia unui punct fata de o triangulare
 * <p>
 * <p>
 * <p>
 * <p>
 * INPUT:  O multime M de puncte, reprezentand varfurile unui triunghi
 * si puncte ın interiorul acestuia.
 * Un punct A.
 * <p>
 * OUTPUT: O triangulare T a lui M.
 * Pozitia lui A fata de T.
 * (pe una din laturi, intr-un triunghi sau in exterior).
 * <p>
 * <p>
 * <p>
 * <p>
 * IDEE:  Orice triangulare poate fi adusa la o forma optima printr-o
 * secventa de 'flip-uri'. Asa ca primul pas este a construi
 * o triangulare, apoi de a aplica algoritmul de flip.
 * <p>
 * TRIANGULARE:
 * initializam T cu triunghiul cel mare
 * pentru fiecare punct D ramas in M:
 * cautam triunghiul ABC in care se afla D
 * stergem ABC
 * adaugam ABD, ACD si BCD
 * <p>
 * FLIP:
 * pentru fiecare triunghi ABC din T
 * pentru fiecare triunghi DEF care difera de ABC printr-un singur punct D
 * daca D este in interiorul circumcentrului triunghiului ABC
 * inversam diagonala din patrulaterul ABCD
 */
package com.company;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;
import java.util.List;

public class Main {

    public static void main(String[] args) {

        /*

        input:
        - point to check
        - points of the big triangle
        - points inside the big triangle


        example:

        50 40

        50 100
        0 0
        100 0

        70 40
        35 40
        50 20

         */


        new Triangulator("input.txt");

    }

}

class Triangulator {

    // region Utils

    static class Point {
        double x, y;

        Point(double x, double y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public String toString() {
            return "(" + (int) x + ", " + (int) y + ")";
        }

        @Override
        public boolean equals(Object obj) {
            return obj.getClass() == this.getClass() &&
                    ((Point) obj).x == x && ((Point) obj).y == y;
        }
    }

    static class Segment {
        Point A, B;

        Segment(Point A, Point B) {
            this.A = A;
            this.B = B;
        }

        @Override
        public String toString() {
            return "[ " + A + " " + B + " ]";
        }

        boolean contains(Point C) {

            final double epsilon = 0.0001;

            double crossProduct = (C.y - A.y) * (B.x - A.x) - (C.x - A.x) * (B.y - A.y);
            if (Math.abs(crossProduct) > epsilon)
                return false;

            double dotProduct = (C.x - A.x) * (B.x - A.x) + (C.y - A.y) * (B.y - A.y);
            if (dotProduct < 0)
                return false;

            double squaredLengthAB = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);

            return dotProduct <= squaredLengthAB;
        }
    }

    private static class Triangle {
        Point A, B, C;

        Triangle(Point A, Point B, Point C) {
            if (A.equals(B) || B.equals(C) || C.equals(A)) {
                System.err.println("The vertices of a triangle must differ one from another!");
                return;
            }
            this.A = A;
            this.B = B;
            this.C = C;
        }

        @Override
        public String toString() {
            return A.toString() + B + C;
        }

        boolean contains(Point P) {
            // The point P is inside the triangle ABC
            // iff it is to the same side for all three segments
            return side(A, B, P) == side(B, C, P) &&
                    side(B, C, P) == side(C, A, P);
        }

        /**
         * @return which side the point R is in relation to the
         *         oriented segment PQ
         *
         *         false means to the right
         *         true means to the left
         */
        static boolean side(Point P, Point Q, Point R) {

            /*
              Determinant
              |    1   1   1   |
              |    Px  Qx  Rx  |
              |    Py  Qy  Ry  |
             */
            double delta = Q.x * R.y + P.x * Q.y + P.y * R.x -
                    (Q.x * P.y + R.x * Q.y + R.y * P.x);

            return delta > 0;
        }

        Circle circumCircle() {
            Line perpBis1 = findPerpendicularBisector(A, B);
            Line perpBis2 = findPerpendicularBisector(B, C);
            Point C = intersectionPoint(perpBis1, perpBis2);
            double r = distance(C, A);
            return new Circle(C, r);
        }

        static Line findPerpendicularBisector(Point A, Point B) {

            Point middle = new Point((A.x + B.x) / 2, (A.y + B.y) / 2);
            double segmentSlope = (B.y - A.y) / (B.x - A.x);
            double bisectorSlope = -1 / segmentSlope;

            double a, b, c;

            // Slope is zero
            if (segmentSlope == 0) {
                //System.out.println("Slope is zero");
                a = 1;
                b = 0;
                c = -middle.x;
            }
            // Slope is infinity
            else if (Double.isInfinite(segmentSlope)) {
                //System.out.println("Slope is infinity");
                a = 0;
                b = 1;
                c = -middle.y;
            }
            // General case
            else {

                a = -bisectorSlope;
                b = 1;
                c = -middle.y + bisectorSlope * middle.x;
            }

            return new Line(a, b, c);
        }

        static Point intersectionPoint(Line L1, Line L2) {
            double delta = L1.a * L2.b - L2.a * L1.b;
            if (delta == 0)
                System.out.println("Paralel lines, can't divide by delta = zero");
            return new Point((L1.b * L2.c - L2.b * L1.c) / delta,
                    (L2.a * L1.c - L1.a * L2.c) / delta);
        }

        List<Point> differentPointsFrom(Triangle other) {

            List<Point> diffPoints = new ArrayList<Point>();
            diffPoints.add(other.A);
            diffPoints.add(other.B);
            diffPoints.add(other.C);

            diffPoints.remove(A);
            diffPoints.remove(B);
            diffPoints.remove(C);

            return diffPoints;
        }

        @Override
        public boolean equals(Object obj) {
            return obj instanceof Triangle &&
                    ((Triangle) obj).A == A &&
                    ((Triangle) obj).B == B &&
                    ((Triangle) obj).C == C;
        }
    }

    private static class Line {

        double a, b, c;

        Line(double A, double B, double C) {
            a = A;
            b = B;
            c = C;
        }

        /*
        public Line (Point A, Point B) {
            a = B.y - A.y;
            b = A.x - B.x;
            c = A.x * (A.y - B.y) + A.y * (B.x - A.x);
        }
        */

        @Override
        public String toString() {
            return a + "x + " + b + "y + " + c;
        }
    }

    private static class Circle {
        Point C;
        double r;

        Circle(Point C, double r) {
            this.C = C;
            this.r = r;
        }

        @Override
        public String toString() {
            return "(" + C + ", " + r + ")";
        }

        // INSIDE the circle, not ON the circle
        boolean contains(Point P) {
            return distance(C, P) < r;
        }
    }

    private static double distance(Point A, Point B) {
        return Math.sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
    }

    // endregion Utils

    private Point toCheck;
    private List<Point> points;
    private List<Triangle> triangulation;

    private TriangulationFrame frame;

    private boolean pauseThreadFlag = false;
    private boolean runUntilEnd = false;
    private boolean abortEverything = false;
    private boolean done = false;
    private final Object notifier = new Object();

    Triangulator(String inputPath) {

        readPoints(inputPath);
        assignButtons();

        // Loop ends on window close
        //noinspection InfiniteLoopStatement
        while (true) {

            buildTriangulation();
            flipEdges();
            relativePosition(toCheck);

            stop();
        }

    }

    private void readPoints(String inputPath) {

        try {
            Scanner inFile = new Scanner(new File(inputPath));

            // TODO: check input:
            // - the first three points are a triangle
            // - each point after that is inside that triangle
            // - check if points are distinct
            // - enough points for initial triangle at least

            toCheck = new Point(inFile.nextDouble(), inFile.nextDouble());

            // Read the points one by one from the file
            points = new ArrayList<Point>();
            while (inFile.hasNextLine()) {
                points.add(new Point(
                        inFile.nextDouble(),
                        inFile.nextDouble()));
            }


            inFile.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

    }

    private void buildTriangulation() {

        // Delete previous triangulation and reinitialise
        triangulation = new ArrayList<Triangle>();

        // Delete previous points drawn
        frame.graphicsComponent.updatePoints(new ArrayList<Point>());

        // Points of the encompassing triangle
        triangulation.add(new Triangle(
                points.get(0),
                points.get(1),
                points.get(2)
        ));

        // Go through each remaining point
        for (int i = 3; i < points.size(); i++) {

            frame.graphicsComponent.updateLines(getEdges());
            pauseThread();
            if (abortEverything)
                return;

            Point D = points.get(i);

            // Search for the triangle that contains the point
            for (int j = 0; j < triangulation.size(); j++) {
                Triangle triangle = triangulation.get(j);

                // Add the automatic three triangles
                if (triangle.contains(D)) {
                    //System.out.println(D + " is contained in " + triangle);
                    triangulation.add(new Triangle(triangle.A, D, triangle.B));
                    triangulation.add(new Triangle(triangle.A, D, triangle.C));
                    triangulation.add(new Triangle(triangle.B, D, triangle.C));
                    // Remove the super-triangle (that was broken into three sub-triangles)
                    triangulation.remove(j);
                    break;
                }
            }
        }
    }

    private void flipEdges() {

        if (abortEverything)
            return;

        // Consider each triangle
        for (int i = 0; i < triangulation.size(); i++) {
            Triangle triangle = triangulation.get(i);

            // Search for one that has a common edge
            for (int j = 0; j < triangulation.size(); j++) {
                Triangle otherTriangle = triangulation.get(j);

                // Two triangles share ONE edge iff they differ by one point
                if (triangle.differentPointsFrom(otherTriangle).size() == 1) {

                    Circle circumcircle = triangle.circumCircle();
                    Point otherDifferent = triangle.differentPointsFrom(otherTriangle).get(0);

                    // Flip
                    if (circumcircle.contains(otherDifferent)) {

//                        System.out.println(triangle + " shares an edge with " + otherTriangle +
//                                " \n\tthey differ by " + different + " circumCircle is " + circumCircle +
//                                " \n\tdistance = " + distance(circumCircle.C, different) + "\n");
//                        System.out.println("i = " + i + " j = " + j);

                        frame.graphicsComponent.updateLines(getEdges());
                        pauseThread();
                        if (abortEverything)
                            return;


                        Point A = triangle.A;
                        Point B = triangle.B;
                        Point C = triangle.C;

                        triangulation.add(new Triangle(A, B, otherDifferent));
                        triangulation.add(new Triangle(B, C, otherDifferent));

                        triangulation.remove(triangle);
                        triangulation.remove(otherTriangle);
                        j++;
                        //break;

                    }
                }
            }
        }
    }

    private List<Segment> getEdges() {

        // We use a set instead of a list for faster containing checking
        Set<Segment> edgeSet = new HashSet<Segment>();

        for (Triangle triangle : triangulation) {
            if (!edgeSet.contains(new Segment(triangle.A, triangle.B)))
                edgeSet.add(new Segment(triangle.A, triangle.B));

            if (!edgeSet.contains(new Segment(triangle.B, triangle.C)))
                edgeSet.add(new Segment(triangle.B, triangle.C));

            if (!edgeSet.contains(new Segment(triangle.C, triangle.A)))
                edgeSet.add(new Segment(triangle.C, triangle.A));

        }

        List<Segment> edges = new ArrayList<Segment>();
        edges.addAll(edgeSet);
        return edges;
    }

    private void relativePosition(Point A) {

        if (abortEverything)
            return;

        boolean foundSomewhere = false;

        // First check if the point is inside one of the triangles
        for (Triangle triangle : triangulation)
            if (triangle.contains(A)) {
                System.out.println(A + " is contained in the triangle " + triangle);
                foundSomewhere = true;
                break;
            }

        // If it is not, check if it is on one of the edges
        if (!foundSomewhere) {
            List<Segment> edges = getEdges();
            for (Segment segment : edges)
                if (segment.contains(A)) {
                    System.out.println(A + " is on the segment " + segment);
                    foundSomewhere = true;
                    break;
                }
        }

        // If it's not on any edges either, it is outside
        if (!foundSomewhere)
            System.out.println(A + " is outside the triangulation");

        ArrayList<Point> toShow = new ArrayList<Point>();
        toShow.add(toCheck);
        frame.graphicsComponent.updatePoints(toShow);

    }

    // region Interface

    private void assignButtons() {

        frame = new TriangulationFrame(new ArrayList<Segment>());

        frame.resetButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                reset();
            }
        });

        frame.stepButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (pauseThreadFlag) {
                    //System.out.println("\tstep => resume thread");
                    if (done)
                        System.out.println("Triangulation done! Press [reset] to restart.");
                    else
                        resumeThread();
                } else
                    System.out.println("Step pressed step while not paused");
            }
        });

        frame.endButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                runUntilEnd = true;
                resumeThread();
            }
        });

    }

    private void pauseThread() {

        if (runUntilEnd)
            return;

        pauseThreadFlag = true;

        synchronized (notifier) {

            while (pauseThreadFlag)
                try {
                    notifier.wait();
                } catch (InterruptedException e) {
                    System.out.println("Interrupt exception");
                }

        }
    }

    private void resumeThread() {
        synchronized (notifier) {

            pauseThreadFlag = false;
            notifier.notify();

        }
    }

    private void reset() {

        // Reset flags
        pauseThreadFlag = false;
        runUntilEnd = false;
        done = false;

        // Resume execution with the abort flag on
        abortEverything = true;
        resumeThread();


    }

    private void stop() {
        runUntilEnd = false;

        if (!abortEverything) {
            done = true;
            pauseThread();
        }

        abortEverything = false;
    }

    // endregion Interface

}

class GraphicsComponent extends JComponent {

    private final static int WIDTH = 500, HEIGHT = 500;
    private final static int POINT_DIAMETER = 10;

    private double maxX, maxY;
    private List<Triangulator.Segment> edges = null;
    private List<Triangulator.Point> vertices = null;

    GraphicsComponent(List<Triangulator.Segment> segments) {
        super();
        setPreferredSize(new Dimension(WIDTH, HEIGHT));
        updateLines(segments);
    }

    @Override
    protected void paintComponent(Graphics g) {

        // Coloring the background
        g.setColor(Color.white);
        g.fillRect(0, 0, getWidth(), getHeight());

        // Draw each line
        g.setColor(Color.black);
        if (edges != null)
            for (Triangulator.Segment segment : edges)
                g.drawLine(
                        (int) (WIDTH * .05 + segment.A.x),
                        (int) (HEIGHT * .95 - segment.A.y),
                        (int) (WIDTH * .05 + segment.B.x),
                        (int) (HEIGHT * .95 - segment.B.y)
                );

        // Show each point (as a little circle)
        g.setColor(Color.red);
        if (vertices != null)
            for (Triangulator.Point point : vertices)
                g.fillOval(
                        -POINT_DIAMETER / 2 + (int) (WIDTH * .05 + point.x),
                        -POINT_DIAMETER / 2 + (int) (HEIGHT * .95 - point.y),
                        POINT_DIAMETER,
                        POINT_DIAMETER
                );
    }

    void updateLines(List<Triangulator.Segment> segments) {

        // Normalizing the points to WIDTH and HEIGHT
        maxX = maxY = 0;
        for (Triangulator.Segment segment : segments) {

            if (segment.A.x > maxX) maxX = segment.A.x;
            if (segment.B.x > maxX) maxX = segment.B.x;

            if (segment.A.y > maxY) maxY = segment.A.y;
            if (segment.B.y > maxY) maxY = segment.B.y;
        }

        edges = new ArrayList<Triangulator.Segment>();
        for (Triangulator.Segment segment : segments)
            edges.add(new Triangulator.Segment(
                    new Triangulator.Point(segment.A.x / maxX * (WIDTH * .9), segment.A.y / maxY * (HEIGHT * .9)),
                    new Triangulator.Point(segment.B.x / maxX * (WIDTH * .9), segment.B.y / maxY * (HEIGHT * .9))
            ));

        repaint();
    }

    void updatePoints(List<Triangulator.Point> points) {

        vertices = new ArrayList<Triangulator.Point>();
        for (Triangulator.Point point : points)
            vertices.add(new Triangulator.Point(
                    point.x / maxX * (WIDTH * .9),
                    point.y / maxY * (HEIGHT * .9)
            ));

        repaint();
    }

}

class TriangulationFrame {

    private JFrame frame;

    JButton resetButton;
    JButton stepButton;
    JButton endButton;

    GraphicsComponent graphicsComponent;


    TriangulationFrame(List<Triangulator.Segment> edges) {

        frame = new JFrame();
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setTitle("Triangluation");

        frame.pack();
        frame.setVisible(true);

        showButtons();

        graphicsComponent = new GraphicsComponent(edges);
        frame.getContentPane().add(graphicsComponent, BorderLayout.CENTER);
        frame.pack();
    }

    private void showButtons() {

        JPanel buttonsPanel = new JPanel();

        resetButton = new JButton("Reset");
        buttonsPanel.add(resetButton);

        stepButton = new JButton("Step");
        buttonsPanel.add(stepButton);

        endButton = new JButton("End");
        buttonsPanel.add(endButton);


        frame.getContentPane().add(buttonsPanel, BorderLayout.SOUTH);
        frame.pack();
    }

}