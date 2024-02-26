#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

const long long INF = 1e18;

// Структура представляет собой ребро графа
struct Edge {
    int adj_vertex; // Смежная вершина
    long long length; // Длина ребра
    bool in_short_path; // Флаг, указывающий, находится ли ребро в кратчайшем пути
    Edge(int vertex, long long len, bool flag = false) : adj_vertex(vertex), length(len), in_short_path(flag) {};
};

// Структура представляет информацию о пути до вершины
struct Path {
    long long dist; // Расстояние до вершины
    int from; // Предыдущая вершина пути
    int edge_id; // Индекс ребра на пути
    Path(long long distance = INF, int v = -1, int e = -1) : dist(distance), from(v), edge_id(e) {}
    
    // Перегрузка оператора < для приоритетной очереди
    bool operator<(const Path& other) const {
        return dist < other.dist;
    }
};

// Класс для нахождения кратчайших путей в графе
class ShortestPath {
public:
    ShortestPath(vector<vector<Edge>> &g) : graph(g) {
        num_of_vertex = graph.size();
    }

    // Второй кратчайший путь в графе
    void findSecondPath(int start_vertex, int finish_vertex) {
        // Первый кратчайший путь в графе
        vector<Path> first = findShortestPath(start_vertex, finish_vertex);

        // Отмечает ребра первого кратчайшего пути
        Path cur = first[finish_vertex];
        while (cur.from != -1) {
            graph[cur.from][cur.edge_id].in_short_path = true;
            cur = first[cur.from];
        }
        // Добавляем дополнительные вершины и ребра в граф для поиска второго пути
        buildSecondLayer();

        // Второй кратчайший путь
        vector<Path> second = findShortestPath(start_vertex + num_of_vertex, finish_vertex);
        second_path_len = second[finish_vertex].dist;
        second_path = getPath(second, finish_vertex);
    }

    // Возвращает длину второго кратчайшего пути
    long long getSecondLength() const {
        return second_path_len;
    }

    // Возвращает второй кратчайший путь 
    vector<int> getSecondPath() const {
        return second_path;
    }

private:
    // Находит кратчайший путь в графе
    vector<Path> findShortestPath(int start_v, int finish_v) {
        vector<Path> path(graph.size(), Path()); // Инициализируем пути до каждой вершины как бесконечность
        priority_queue<Path> q; // Приоритетная очередь для выбора вершин с наименьшим расстоянием
        path[start_v] = Path(0); // Расстояние до стартовой вершины равно 0
        q.push(Path(0, start_v)); // Добавляем стартовую вершину в очередь

        // Применяем алгоритм Дейкстры
        while (!q.empty()) {
            int vertex = q.top().from; // Берем вершину с наименьшим расстоянием из очереди
            long long dist = q.top().dist;
            q.pop();

            if (dist > path[vertex].dist) continue; // Если текущее расстояние до вершины больше, чем уже известное, пропускаем

            // Обновляем расстояния до всех смежных вершин
            for (int i = 0; i < graph[vertex].size(); i++) {
                int to = graph[vertex][i].adj_vertex;
                long long new_dist = graph[vertex][i].length + dist;
                if (path[to].dist > new_dist) {
                    path[to] = Path(new_dist, vertex, i);
                    q.push(Path(new_dist, to)); // Добавляем обновленную вершину в очередь
                }
            }
        }

        return path; // Возвращаем массив кратчайших путей
    }

    // Добавляет дополнительные вершины и ребра в граф для поиска второго кратчайшего пути
    void buildSecondLayer() {
        for (int i = 0; i < num_of_vertex; i++) {
            vector<Edge> new_vertex = graph[i];
            for (int j = 0; j < graph[i].size(); j++) {
                int ind = j;
                if (!graph[i][j].in_short_path) {
                    new_vertex.push_back(new_vertex[j]);
                    ind = new_vertex.size() - 1;
                }
                new_vertex[ind].adj_vertex += num_of_vertex;
            }
            graph.push_back(move(new_vertex));
        }
    }

    // Восстанавливает путь из предыдущих вершин
    vector<int> getPath(const vector<Path> &prev, int finish) {
        vector<int> path;
        int v = finish;
        while (v != -1) {
            int ind = (v >= num_of_vertex) ? v - num_of_vertex : v;
            path.push_back(ind + 1); // Добавляет вершину в путь
            v = prev[v].from; // Переходим к предыдущей вершине
        }
        reverse(path.begin(), path.end()); // Переворачиваем путь
        return path;
    }

    int num_of_vertex; // Количество вершин в исходном графе
    long long second_path_len; // Длина второго кратчайшего пути
    vector<int> second_path; // Второй кратчайший путь
    vector<vector<Edge>> graph; // Граф
};

int main()
{
    ifstream fin("input.in");
    ofstream fout("output.out");
    ios_base::sync_with_stdio(false); fin.tie(NULL); fout.tie(NULL);
    
    int num_of_vertex, num_of_edges;
    fin >> num_of_vertex >> num_of_edges;

    int u, v;
    long long dist;
    vector<vector<Edge>> graph(num_of_vertex);
    for (int i = 0; i < num_of_edges; i++) {
        fin >> v >> u >> dist;
        graph[--v].push_back(Edge(--u, dist));
    }

    ShortestPath shortest_path(graph);

    int start_vertex, finish_vertex;
    fin >> start_vertex >> finish_vertex;

    shortest_path.findSecondPath(start_vertex - 1, finish_vertex - 1);

    fout << shortest_path.getSecondLength() << "\n";
    vector<int> path = shortest_path.getSecondPath();
    for (int i = 0; i < path.size(); i++) {
        fout << path[i];
        if (i < path.size() - 1) {
            fout << " ";
        }
    }

    return 0;
}
