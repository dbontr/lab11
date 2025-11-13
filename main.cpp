#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <string>

class Matrix {
private:
    int n;                     // dimension (n x n)
    std::vector<int> data;     // flat storage: row-major, size n*n

public:
    // Default constructor
    Matrix() : n(0) {}

    // Construct n x n matrix, initialized to zeros
    Matrix(int size) : n(size), data(size * size, 0) {}

    // Accessors
    int size() const {
        return n;
    }

    // Element access (with basic bounds checking in debug spirit)
    int& at(int row, int col) {
        return data[row * n + col];
    }

    int at(int row, int col) const {
        return data[row * n + col];
    }

    // Pretty-print the matrix with aligned columns
    void print(std::ostream& os) const {
        if (n == 0) {
            os << "[empty matrix]\n";
            return;
        }

        // Find maximum width for formatting
        int maxWidth = 0;
        for (int i = 0; i < n * n; ++i) {
            int value = data[i];
            int width = static_cast<int>(std::to_string(value).size());
            if (width > maxWidth) maxWidth = width;
        }
        if (maxWidth < 2) maxWidth = 2;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                os << std::setw(maxWidth + 1) << at(i, j);
            }
            os << '\n';
        }
    }

    // Sum of main diagonal (i, i)
    int sumMainDiagonal() const {
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            sum += at(i, i);
        }
        return sum;
    }

    // Sum of secondary diagonal (i, n-1-i)
    int sumSecondaryDiagonal() const {
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            sum += at(i, n - 1 - i);
        }
        return sum;
    }

    // Matrix addition: this + other
    Matrix operator+(const Matrix& other) const {
        if (n != other.n) {
            throw std::runtime_error("Matrix sizes do not match for addition.");
        }
        Matrix result(n);
        for (int i = 0; i < n * n; ++i) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    // Matrix multiplication: this * other
    Matrix operator*(const Matrix& other) const {
        if (n != other.n) {
            throw std::runtime_error("Matrix sizes do not match for multiplication.");
        }
        Matrix result(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int sum = 0;
                for (int k = 0; k < n; ++k) {
                    sum += at(i, k) * other.at(k, j);
                }
                result.at(i, j) = sum;
            }
        }
        return result;
    }

    // Read N*N integers into the matrix (N must already be set)
    friend std::istream& operator>>(std::istream& is, Matrix& m);
};

// Stream extraction: read existing-size matrix from stream
std::istream& operator>>(std::istream& is, Matrix& m) {
    for (int i = 0; i < m.n; ++i) {
        for (int j = 0; j < m.n; ++j) {
            if (!(is >> m.at(i, j))) {
                return is;
            }
        }
    }
    return is;
}

// Swap rows in a copy (pass-by-value to avoid changing original matrix)
// Default rows: 0 and 1
Matrix swapRows(Matrix m, int r1 = 0, int r2 = 1) {
    int n = m.size();
    if (r1 < 0 || r1 >= n || r2 < 0 || r2 >= n) {
        std::cerr << "Invalid row indices for swapRows. No swap performed.\n";
        return m;
    }
    if (r1 == r2) {
        return m; // nothing to do
    }

    for (int j = 0; j < n; ++j) {
        int temp = m.at(r1, j);
        m.at(r1, j) = m.at(r2, j);
        m.at(r2, j) = temp;
    }
    return m;
}

// Swap columns in a copy (pass-by-value to avoid changing original matrix)
// Default columns: 0 and 1
Matrix swapColumns(Matrix m, int c1 = 0, int c2 = 1) {
    int n = m.size();
    if (c1 < 0 || c1 >= n || c2 < 0 || c2 >= n) {
        std::cerr << "Invalid column indices for swapColumns. No swap performed.\n";
        return m;
    }
    if (c1 == c2) {
        return m; // nothing to do
    }

    for (int i = 0; i < n; ++i) {
        int temp = m.at(i, c1);
        m.at(i, c1) = m.at(i, c2);
        m.at(i, c2) = temp;
    }
    return m;
}

// Update a single element in a copy (defaults row=0, col=0, value=100)
Matrix updateElement(Matrix m, int row = 0, int col = 0, int value = 100) {
    int n = m.size();
    if (row < 0 || row >= n || col < 0 || col >= n) {
        std::cerr << "Invalid indices for updateElement. No update performed.\n";
        return m;
    }
    m.at(row, col) = value;
    return m;
}

int main() {
    std::string filename;
    std::cout << "Enter input filename: ";
    std::cin >> filename;

    std::ifstream fin(filename.c_str());
    if (!fin) {
        std::cerr << "Error: could not open file '" << filename << "'.\n";
        return 1;
    }

    int N;
    if (!(fin >> N) || N <= 0) {
        std::cerr << "Error: first value in file must be a positive integer N.\n";
        return 1;
    }

    Matrix A(N), B(N);

    // Read the two N x N matrices
    fin >> A;
    fin >> B;

    if (!fin) {
        std::cerr << "Error: not enough matrix data in file.\n";
        return 1;
    }

    std::cout << "\nMatrix A:\n";
    A.print(std::cout);

    std::cout << "\nMatrix B:\n";
    B.print(std::cout);

    // 2. Addition using operator overloading
    try {
        Matrix C = A + B;
        std::cout << "\nA + B:\n";
        C.print(std::cout);
    } catch (const std::exception& e) {
        std::cerr << "Addition error: " << e.what() << "\n";
    }

    // 3. Multiplication using operator overloading
    try {
        Matrix D = A * B;
        std::cout << "\nA * B:\n";
        D.print(std::cout);
    } catch (const std::exception& e) {
        std::cerr << "Multiplication error: " << e.what() << "\n";
    }

    // 4. Diagonal sums for A (you could also do for B if you want)
    std::cout << "\nDiagonal sums for Matrix A:\n";
    std::cout << "Main diagonal sum:      " << A.sumMainDiagonal() << "\n";
    std::cout << "Secondary diagonal sum: " << A.sumSecondaryDiagonal() << "\n";

    // 5. Swap rows (by value)
    std::cout << "\nEnter two row indices to swap (0-based, default 0 1): ";
    int r1 = 0, r2 = 1;
    if (!(std::cin >> r1 >> r2)) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        r1 = 0; r2 = 1;
        std::cout << "Using default row indices 0 and 1.\n";
    }
    Matrix A_rows_swapped = swapRows(A, r1, r2);
    std::cout << "\nMatrix A with rows " << r1 << " and " << r2 << " swapped:\n";
    A_rows_swapped.print(std::cout);

    // 6. Swap columns (by value)
    std::cout << "\nEnter two column indices to swap (0-based, default 0 1): ";
    int c1 = 0, c2 = 1;
    if (!(std::cin >> c1 >> c2)) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        c1 = 0; c2 = 1;
        std::cout << "Using default column indices 0 and 1.\n";
    }
    Matrix A_cols_swapped = swapColumns(A, c1, c2);
    std::cout << "\nMatrix A with columns " << c1 << " and " << c2 << " swapped:\n";
    A_cols_swapped.print(std::cout);

    // 7. Update element (by value)
    std::cout << "\nEnter row, column, and new value to update (default 0 0 100): ";
    int ur = 0, uc = 0, val = 100;
    if (!(std::cin >> ur >> uc >> val)) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        ur = 0; uc = 0; val = 100;
        std::cout << "Using default (row=0, col=0, value=100).\n";
    }
    Matrix A_updated = updateElement(A, ur, uc, val);
    std::cout << "\nMatrix A after update at (" << ur << ", " << uc << ") = " << val << ":\n";
    A_updated.print(std::cout);

    std::cout << "\nOriginal Matrix A (unchanged):\n";
    A.print(std::cout);

    return 0;
}
