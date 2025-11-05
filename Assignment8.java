//Name - Chaitanya Deore
// PRN - 123B1F020

import java.util.*;

class Node implements Comparable<Node> {
    int[][] reducedMatrix;
    List<Integer> path;
    int cost;
    int vertex;
    int level;

    Node(int[][] matrix, List<Integer> path, int cost, int vertex, int level) {
        this.reducedMatrix = matrix;
        this.path = path;
        this.cost = cost;
        this.vertex = vertex;
        this.level = level;
    }

    @Override
    public int compareTo(Node other) {
        return this.cost - other.cost;
    }
}

public class Assignment8 {

    static final int INF = 999999;

    static int reduceMatrix(int[][] matrix) {
        int reduction = 0;
        int n = matrix.length;

        for (int i = 0; i < n; i++) {
            int rowMin = INF;
            for (int j = 0; j < n; j++)
                if (matrix[i][j] < rowMin)
                    rowMin = matrix[i][j];

            if (rowMin != INF && rowMin != 0) {
                reduction += rowMin;
                for (int j = 0; j < n; j++)
                    if (matrix[i][j] != INF)
                        matrix[i][j] -= rowMin;
            }
        }

        for (int j = 0; j < n; j++) {
            int colMin = INF;
            for (int i = 0; i < n; i++)
                if (matrix[i][j] < colMin)
                    colMin = matrix[i][j];

            if (colMin != INF && colMin != 0) {
                reduction += colMin;
                for (int i = 0; i < n; i++)
                    if (matrix[i][j] != INF)
                        matrix[i][j] -= colMin;
            }
        }

        return reduction;
    }

    static Node newNode(int[][] parentMatrix, List<Integer> path, int level, int i, int j) {
        int n = parentMatrix.length;
        int[][] childMatrix = new int[n][n];

        for (int k = 0; k < n; k++)
            childMatrix[k] = parentMatrix[k].clone();

        for (int k = 0; k < n; k++) {
            childMatrix[i][k] = INF;
            childMatrix[k][j] = INF;
        }
        childMatrix[j][0] = INF; 

        int cost = parentMatrix[i][j] + reduceMatrix(childMatrix);

        List<Integer> newPath = new ArrayList<>(path);
        newPath.add(j);

        return new Node(childMatrix, newPath, cost + parentMatrix[i][j], j, level + 1);
    }

    static void branchAndBound(int[][] costMatrix) {
        int n = costMatrix.length;

        int[][] reducedMatrix = new int[n][n];
        for (int i = 0; i < n; i++)
            reducedMatrix[i] = costMatrix[i].clone();

        int rootCost = reduceMatrix(reducedMatrix);
        List<Integer> path = new ArrayList<>();
        path.add(0);

        PriorityQueue<Node> pq = new PriorityQueue<>();
        pq.add(new Node(reducedMatrix, path, rootCost, 0, 0));

        int minCost = INF;
        List<Integer> bestPath = new ArrayList<>();

        while (!pq.isEmpty()) {
            Node min = pq.poll();

            if (min.level == n - 1) {
                min.path.add(0);
                int totalCost = min.cost + costMatrix[min.vertex][0];
                if (totalCost < minCost) {
                    minCost = totalCost;
                    bestPath = min.path;
                }
                continue;
            }

            for (int j = 0; j < n; j++) {
                if (costMatrix[min.vertex][j] != INF && !min.path.contains(j)) {
                    Node child = newNode(min.reducedMatrix, min.path, min.level, min.vertex, j);
                    pq.add(child);
                }
            }
        }

        System.out.println("\nOptimal Delivery Route:");
        for (int i = 0; i < bestPath.size(); i++) {
            System.out.print("City " + (bestPath.get(i) + 1));
            if (i < bestPath.size() - 1) System.out.print(" -> ");
        }
        System.out.println("\nMinimum Delivery Cost: " + minCost);
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter number of cities: ");
        int n = sc.nextInt();

        int[][] distance = new int[n][n];
        double[] fuelMultiplier = new double[n];  

        System.out.println("Enter distance matrix (0 for same city):");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                distance[i][j] = sc.nextInt();
                if (i == j)
                    distance[i][j] = INF;
            }
        }

        System.out.println("Enter fuel multiplier for each city (e.g., 1.0, 1.2, etc.):");
        for (int i = 0; i < n; i++)
            fuelMultiplier[i] = sc.nextDouble();

        int[][] costMatrix = new int[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                costMatrix[i][j] = (int) Math.round(distance[i][j] * fuelMultiplier[i]);

        branchAndBound(costMatrix);
    }
}

