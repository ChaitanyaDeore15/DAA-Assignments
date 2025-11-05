//Name - Chaitanya Deore
// PRN - 123B1F020
import java.util.*;

class Graph {
    int n;
    List<List<Integer>> adj;

    Graph(int n) {
        this.n = n;
        adj = new ArrayList<>();
        for (int i = 0; i < n; i++) adj.add(new ArrayList<>());
    }

    void addEdge(int u, int v) {
        if (!adj.get(u).contains(v)) {
            adj.get(u).add(v);
            adj.get(v).add(u);
        }
    }
}

public class Assignment7 {

    static int[] greedyColoring(Graph g) {
        int n = g.n;
        int[] result = new int[n];
        Arrays.fill(result, -1);
        result[0] = 0;

        for (int u = 1; u < n; u++) {
            boolean[] used = new boolean[n];
            for (int v : g.adj.get(u)) {
                if (result[v] != -1) used[result[v]] = true;
            }
            int cr;
            for (cr = 0; cr < n; cr++)
                if (!used[cr]) break;
            result[u] = cr;
        }
        return result;
    }

    static int[] welshPowell(Graph g) {
        int n = g.n;
        Integer[] order = new Integer[n];
        for (int i = 0; i < n; i++) order[i] = i;

        Arrays.sort(order, (a, b) -> g.adj.get(b).size() - g.adj.get(a).size());
        int[] color = new int[n];
        Arrays.fill(color, -1);
        int currentColor = 0;

        for (int i = 0; i < n; i++) {
            int u = order[i];
            if (color[u] == -1) {
                color[u] = currentColor;
                for (int j = i + 1; j < n; j++) {
                    int v = order[j];
                    if (color[v] == -1) {
                        boolean conflict = false;
                        for (int k : g.adj.get(v)) {
                            if (color[k] == currentColor) {
                                conflict = true;
                                break;
                            }
                        }
                        if (!conflict) color[v] = currentColor;
                    }
                }
                currentColor++;
            }
        }
        return color;
    }

    static int[] DSATUR(Graph g) {
        int n = g.n;
        int[] color = new int[n];
        Arrays.fill(color, -1);
        int[] degree = new int[n];
        int[] sat = new int[n];

        for (int i = 0; i < n; i++)
            degree[i] = g.adj.get(i).size();

        int colored = 0;
        while (colored < n) {
            int u = -1, maxSat = -1, maxDeg = -1;
            for (int i = 0; i < n; i++) {
                if (color[i] == -1) {
                    if (sat[i] > maxSat || (sat[i] == maxSat && degree[i] > maxDeg)) {
                        maxSat = sat[i];
                        maxDeg = degree[i];
                        u = i;
                    }
                }
            }

            boolean[] used = new boolean[n];
            for (int v : g.adj.get(u)) {
                if (color[v] != -1) used[color[v]] = true;
            }
            int c;
            for (c = 0; c < n; c++)
                if (!used[c]) break;
            color[u] = c;
            colored++;

            for (int v : g.adj.get(u)) {
                if (color[v] == -1) {
                    Set<Integer> adjColors = new HashSet<>();
                    for (int k : g.adj.get(v))
                        if (color[k] != -1)
                            adjColors.add(color[k]);
                    sat[v] = adjColors.size();
                }
            }
        }
        return color;
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter number of courses: ");
        int numCourses = sc.nextInt();

        System.out.print("Enter number of students: ");
        int numStudents = sc.nextInt();

        Graph g = new Graph(numCourses);
        sc.nextLine();

        System.out.println("Enter each student's enrolled course IDs (space-separated, end with -1):");
        for (int i = 0; i < numStudents; i++) {
            System.out.print("Student " + (i + 1) + ": ");
            List<Integer> courses = new ArrayList<>();
            while (true) {
                int c = sc.nextInt();
                if (c == -1) break;
                courses.add(c - 1); // convert to 0-based
            }
            for (int a = 0; a < courses.size(); a++)
                for (int b = a + 1; b < courses.size(); b++)
                    g.addEdge(courses.get(a), courses.get(b));
        }

        System.out.print("Enter number of available classrooms: ");
        int rooms = sc.nextInt();

        int[] greedy = greedyColoring(g);
        int[] wp = welshPowell(g);
        int[] dsatur = DSATUR(g);

        int greedySlots = Arrays.stream(greedy).max().getAsInt() + 1;
        int wpSlots = Arrays.stream(wp).max().getAsInt() + 1;
        int dsaturSlots = Arrays.stream(dsatur).max().getAsInt() + 1;

        System.out.println("\n--- Algorithm Comparison ---");
        System.out.println("Greedy used " + greedySlots + " slots");
        System.out.println("Welsh-Powell used " + wpSlots + " slots");
        System.out.println("DSATUR used " + dsaturSlots + " slots");

        int minSlots = Math.min(greedySlots, Math.min(wpSlots, dsaturSlots));
        int[] finalColor;
        String chosenAlgo;
        if (minSlots == dsaturSlots) {
            finalColor = dsatur;
            chosenAlgo = "DSATUR";
        } else if (minSlots == wpSlots) {
            finalColor = wp;
            chosenAlgo = "Welsh-Powell";
        } else {
            finalColor = greedy;
            chosenAlgo = "Greedy";
        }

        System.out.println("\nChosen Algorithm: " + chosenAlgo);
        System.out.println("Total Slots: " + minSlots);

        Map<Integer, List<Integer>> slotCourses = new HashMap<>();
        for (int i = 0; i < numCourses; i++) {
            slotCourses.computeIfAbsent(finalColor[i], k -> new ArrayList<>()).add(i + 1);
        }

        System.out.println("\nRoom Assignments:");
        for (Map.Entry<Integer, List<Integer>> entry : slotCourses.entrySet()) {
            int slot = entry.getKey();
            List<Integer> courses = entry.getValue();
            System.out.println("Slot " + slot + " -> Courses: " + courses);
            for (int i = 0; i < courses.size(); i++) {
                if (i < rooms)
                    System.out.println("  Course " + courses.get(i) + " -> Room " + (i + 1));
                else
                    System.out.println("  Course " + courses.get(i) + " -> Room Unavailable (need more rooms)");
            }
        }
    }
}

