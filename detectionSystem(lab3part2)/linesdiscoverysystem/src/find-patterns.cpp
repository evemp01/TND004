#include <iostream>
#include <format>
#include <compare>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <set>

#include <find-patterns.hpp>

const std::filesystem::path data_dir{DATA_DIR};

constexpr int minPoints = 4;

struct Point { // feel free to modify
public:
    Point(int x = 0, int y = 0) : x_{x}, y_{y} {}

    bool operator==(const Point& p) const = default;

    // Compare by y-coordinates and break ties by x-coordinates
    std::strong_ordering operator<=>(Point p) const {
        if (y_ < p.y_) {
            return std::strong_ordering::less;
        }
        if (y_ > p.y_) {
            return std::strong_ordering::greater;
        }
        if (x_ < p.x_) {
            return std::strong_ordering::less;
        }
        if (x_ > p.x_) {
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equivalent;
    }

    std::string toString() const { return std::format("({},{})", x_, y_); }

    long long x_;
    long long y_;
};

struct Segment {
    Point p1;
    Point p2;

    bool operator<(const Segment& other) const {
        if (p1 < other.p1) return true;
        if (other.p1 < p1) return false;
        return p2 < other.p2;
    }
};

/* ***************************************************** */

std::vector<Point> readPoints(const std::filesystem::path& file) {
    std::ifstream f(file);
    if (!f) {
        std::cout << "No file named: " << file << "\n";
        return {};
    }

    int numOfPoints;
    f >> numOfPoints;
    std::vector<Point> points;
    points.reserve(numOfPoints);
    int x_val; 
    int y_val;
    while (f >> x_val >> y_val) {
        points.push_back({x_val, y_val});
    }

    return points;
}

// Funktion för att beräkna lutningen mellan två punkter
double calculateSlope(Point p1, Point p2) {
    if (p1.x_ == p2.x_) {
        return std::numeric_limits<double>::infinity();  // Vertikal linje
    }
    return static_cast<double>(p2.y_ - p1.y_) / (p2.x_ - p1.x_);
}

int main() {
    std::cout << "Enter the name of input points file: ";
    std::string points_file;
    std::cin >> points_file;

    analyseData(points_file);
}

/* ***************************************************** */

void analyseData(const std::filesystem::path& pointsFile,
                 const std::filesystem::path& segmentsFile,
                 const std::filesystem::path& lineSegmentsFile) {
    
    if (!std::filesystem::exists(pointsFile)) {
        std::cout << "No such file" << pointsFile << "\n";
        return;
    }
    
    std::vector<Point> points = readPoints(pointsFile);

    // Sort the points first by y-coordinates and then x-coordinates
    std::sort(points.begin(), points.end());

    //for (int i = 0; i < std::ssize(points); ++i) {
    //std::cout << points[i].x_ << " " << points[i].y_ << "\n";
    //}

    std::ofstream outfile_segments(segmentsFile);
    if (!outfile_segments.is_open()) {
    std::cerr << "Error opening file: " << segmentsFile << "\n";
    return;
    }
    std::ofstream outfile_line_segments(lineSegmentsFile);
    if (!outfile_line_segments.is_open()) {
    std::cerr << "Error opening file: " << lineSegmentsFile << "\n";
    return;
    }

    std::set<Segment> detectedSegments;

    for (int p = 0; p < std::ssize(points); ++p) {
        Point origin = points[p];
        // Kopiera points vektorn (utan punkt points[p])
        std::vector<Point> otherPoints;
        otherPoints.reserve(std::ssize(points) - 1);
        
       
        for (int i = 0; i < std::ssize(points); ++i) {
            if (i != p) {
                otherPoints.push_back(points[i]);
            }
        }
       
        std::stable_sort(otherPoints.begin(), otherPoints.end(),
                         [&origin](const Point& a, const Point& b) {
                             return calculateSlope(origin, a) < calculateSlope(origin, b);
                         });

        int i = 0;
        while (i < otherPoints.size()) {
            //double slope = calculateSlope(origin, otherPoints[i]);
            std::vector<Point> collinearPoints = {origin};
            Point refPoint = otherPoints[i];
            collinearPoints.push_back(refPoint);
            ++i;

            // Lägg till punkter med samma lutning
            while (i < otherPoints.size() &&
                   ((refPoint.y_ - origin.y_) * (otherPoints[i].x_ - origin.x_)) ==
                   ((otherPoints[i].y_ - origin.y_) * (refPoint.x_ - origin.x_))) {
                collinearPoints.push_back(otherPoints[i]);
                ++i;
            }

            if (collinearPoints.size() >= 4) {
                // Find the lexicographically smallest and largest points
                auto minMax = std::minmax_element(collinearPoints.begin(), collinearPoints.end());
                Segment segment{*(minMax.first), *(minMax.second)};

                if (detectedSegments.find(segment) == detectedSegments.end()) {
                    detectedSegments.insert(segment);
                    outfile_segments << segment.p1.x_ << " " << segment.p1.y_ << " "
                                     << segment.p2.x_ << " " << segment.p2.y_ << "\n";
                    
                    for (int k = 0; k < std::ssize(collinearPoints); ++k) {
                        outfile_line_segments << "(" << collinearPoints[k].x_ << ","
                                              << collinearPoints[k].y_ << ")";
                        if (k != std::ssize(collinearPoints) - 1) {
                            outfile_line_segments << "->";
                        } else {
                            outfile_line_segments << "\n";
                        }
                    }
                }
            }
        }
    }
}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;
    std::filesystem::path line_segments_name = "line-segments-" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name,
                data_dir / "output" / "line-segments" / line_segments_name);
}

