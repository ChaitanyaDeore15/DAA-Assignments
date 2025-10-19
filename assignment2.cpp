#include <bits/stdc++.h>
using namespace std;

struct Movie {
    string title;
    double imdb_rating;
    int release_year;
    double popularity;
};

int partition(vector<Movie>& movies, int low, int high, function<bool(const Movie&, const Movie&)> cmp) {
    int pivotIndex = low + rand() % (high - low + 1);
    swap(movies[pivotIndex], movies[high]);
    Movie pivot = movies[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (cmp(movies[j], pivot)) {
            i++;
            swap(movies[i], movies[j]);
        }
    }
    swap(movies[i + 1], movies[high]);
    return i + 1;
}

void quickSort(vector<Movie>& movies, int low, int high, function<bool(const Movie&, const Movie&)> cmp) {
    if (low < high) {
        int pi = partition(movies, low, high, cmp);
        quickSort(movies, low, pi - 1, cmp);
        quickSort(movies, pi + 1, high, cmp);
    }
}

vector<Movie> generateMovies(int n) {
    vector<Movie> movies(n);
    srand(time(0));
    for (int i = 0; i < n; i++) {
        movies[i].title = "Movie_" + to_string(i + 1);
        movies[i].imdb_rating = (rand() % 1000) / 100.0; 
        movies[i].release_year = 1980 + rand() % 45;    
        movies[i].popularity = rand() % 1000000;      
    }
    return movies;
}

void printMovies(const vector<Movie>& movies, int limit = 10) {
    for (int i = 0; i < min(limit, (int)movies.size()); i++) {
        cout << setw(10) << left << movies[i].title
             << " | Rating: " << movies[i].imdb_rating
             << " | Year: " << movies[i].release_year
             << " | Popularity: " << movies[i].popularity << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 100000; 
    vector<Movie> movies = generateMovies(n);

    cout << "Enter sorting parameter (1=IMDB Rating, 2=Release Year, 3=Popularity): ";
    int choice;
    cin >> choice;

    function<bool(const Movie&, const Movie&)> cmp;

    if (choice == 1)
        cmp = [](const Movie& a, const Movie& b) { return a.imdb_rating > b.imdb_rating; };
    else if (choice == 2)
        cmp = [](const Movie& a, const Movie& b) { return a.release_year > b.release_year; };
    else
        cmp = [](const Movie& a, const Movie& b) { return a.popularity > b.popularity; };

    cout << "\nSorting movies, please wait...\n";

    clock_t start = clock();
    quickSort(movies, 0, n - 1, cmp);
    clock_t end = clock();

    double time_taken = double(end - start) / CLOCKS_PER_SEC;
    cout << "Sorting completed in " << fixed << setprecision(4) << time_taken << " seconds.\n\n";

    cout << "Top 10 movies (sorted):\n";
    printMovies(movies, 10);

    return 0;
}
