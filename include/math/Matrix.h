#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

/**
 * Templated class Matrix.
 * A matrix class that is used to represent matrix like data structures.
 * Contains important functions such as deleteRows(), deleteColumns() and cho() to handle LSEs.
 */
template<typename T>
class Matrix {

    private:

        /**
         * Private member variable.
         * Number of rows
         */
        size_t _size1;

        /**
         * Private member variable.
         * Number of columns
         */
        size_t _size2;

        /**
         * Private member variable.
         * A dynamic matrix structure
         */
        T** _matrix;

    public:

        /**
        * A static public vartible for checking memory leaks (dangling pointers and such).
        * Used in unit tests to compare the number of allocations and destructions.
        * Should not be used for analysis!
        */
        static int allocations;

        /**
        * Matrix class constructor.
        * Generates a Matrix instance.
        */
        Matrix();

        /**
        * Matrix class constructor.
        * Generates an empty Matrix.
        * @param r Number of rows
        * @param c Number of columns
        */
        Matrix(const size_t& r, const size_t& c);

        /**
        * Matrix class constructor.
        * Generates a Matrix with all elements equal to given value.
        * @param r Number of rows
        * @param c Number of columns
        * @param value The wanted value of initialization
        */
        Matrix(const size_t& r, const size_t& c, T value);

        /**
        * Matrix class constructor.
        * Constructs matrices with list syntax.
        * Example: Matrix<int> M = {{0,0},{0,0}};
        */
        Matrix(std::initializer_list<std::initializer_list<T>> init);

        /**
        * Matrix class copy-constructor.
        * @param M Matrix that is wanted to be deep-copied
        */
        Matrix(const Matrix& M);

        /**
        * Matrix class destructor
        */
        ~Matrix();

        /**
        * Member function for finding the size.
        * @return A vector containing the sizes in order: {rows, columns}.
        */
        std::vector<size_t> getSize() const;

        /**
        * Member function for printing a matrix to the console
        */
        void printOut() const;

        /**
        * Member function for operator() overloading.
        * Allows for both read and write.
        * @param r Row index
        * @param c Column index
        */
        T& operator()(const size_t& r, const size_t& c);

        /**
        * Member function for operator() overloading.
        * Allows only for reading.
        * @param r Row index
        * @param c Column index
        */
        const T& operator()(const size_t& r, const size_t& c) const;

        /**
        * Member function for operator = .
        * Deep copies matrices.
        * @param M Matrix that is desired to be copied
        * @return Deep copy matrix
        */
        Matrix<T>& operator=(const Matrix& M);

        /**
        * Member function for operator + .
        * Allows for adding matrices.
        * @param M2 Matrix that is to be added
        * @return Resulting matrix
        */
        Matrix<T> operator+(const Matrix<T>& M2) const;

        /**
        * Member function for operator -.
        * Allows for subtracting matrices.
        * @param M2 Matrix that is to be subtracted
        * @return Resulting matrix
        */
        Matrix<T> operator-(const Matrix<T>& M2) const;

        /**
        * Member function for operator * overloading.
        * Allows for multiplying matrices.
        * @param M2 Matrix that is to be multiplied from right
        * @return Resulting matrix
        */
        Matrix<T> operator*(const Matrix<T>& M2) const;

        /**
        * Member function for operator * overloading.
        * Allows for multiplying matrices with scalars.
        * @param scalar Scalar that is to be multiplied
        * @return Resulting matrix
        */
        Matrix<T> operator*(T scalar) const;

        /**
        * Member function for transposing a matrix.
        * Returns a new matrix with transposed values.
        * @return Transposed matrix
        */
        Matrix<T> transpose() const;

        /**
        * Member function for deleting a row.
        * @param r Row index to be deleted
        */
        void deleteRow(size_t r);

        /**
        * Member function for deleting multiple rows.
        * @param r Vector of row indices to be deleted
        */
        void deleteRows(std::vector<size_t>& r);

        /**
        * Member function for concetanating rows.
        * @param rows New row matrix to be added to the end
        */
        void addRows(Matrix<T>& rows);

        /**
        * Member function for deleting a column.
        * @param c Column index to be deleted
        */
        void deleteColumn(size_t c);

        /**
        * Member function for deleting multiple columns.
        * @param c Vector of column indices to be deleted
        */
        void deleteColumns(std::vector<size_t>& c);

        /**
        * Member function for computing matrix vector multiplication.
        * @param vec Vector that is wanted to be multiplied from right
        * @return Resulting vector
        */
        std::vector<T> mVm(const std::vector<T>& vec) const;

        /**
        * Member function for computing complete Cholesky decompositon.
        * Handy for SPD systems. Uses Cholesky–Banachiewicz algorithm
        * @return Lower triangle matrix
        */
        Matrix<T> cho() const;

        /**
        * Member function for computing inverse of a lower triangular matrix.
        * Used to solve the finite element linear system of equations. Banachiewicz inversion
        * @return Inverse of the lower triangular matrix.
        */
        Matrix<T> L_inverse() const;
};

template<typename T>
int Matrix<T>::allocations = 0;

// TEMPLATE DEFINITIONS, ONLY-HEADER FILE IMPLEMENTATION!

template<typename T>
Matrix<T>::Matrix() : _matrix(nullptr), _size1(0), _size2(0){};

template<typename T>
Matrix<T>::Matrix(const size_t& r,const size_t& c) : _size1(r), _size2(c){

    _matrix = new T*[_size1];
    for (size_t i = 0; i < _size1; ++i){
      _matrix[i] = new T[_size2];
      allocations++;
    };
};

template<typename T>
Matrix<T>::Matrix(const size_t& r,const size_t& c, T value) : _size1(r), _size2(c){

    _matrix = new T*[_size1];
    for (size_t i = 0; i < _size1; ++i){
      _matrix[i] = new T[_size2];
      allocations++;
    };

    for (size_t i = 0; i < _size1; ++i){
        for (size_t j = 0; j < _size2; ++j){
            _matrix[i][j] = value;
        }
    }
};

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> init){

    _size1 = init.size();
    _size2 = init.begin()->size();

    size_t m = 0;
    for (const auto& row : init) {
        if (row.size() != _size2)
            throw std::runtime_error("Rows must all have equal size");
        m++;
    }

    _matrix = new T*[_size1];
    for (size_t i = 0; i < _size1; ++i){
      _matrix[i] = new T[_size2];
      allocations++;
    };

    size_t i = 0;
        for (const auto& row : init) {

            size_t j = 0;
            for (const auto& value : row) {
                _matrix[i][j++] = value;
            }
            i++;
        }
};

template<typename T>
Matrix<T>::Matrix(const Matrix& M)
    : _size1(M._size1), _size2(M._size2)
{
    _matrix = new T*[_size1];
    for (size_t i = 0; i < _size1; ++i) {
        _matrix[i] = new T[_size2];
        std::copy(M._matrix[i], M._matrix[i] + _size2, _matrix[i]);
        allocations++;
    }
}

template<typename T>
Matrix<T>::~Matrix() {
    for (size_t i = 0; i < _size1; ++i){
        delete[] _matrix[i];
        allocations--;
    };
    delete[] _matrix;
}

template<typename T>
std::vector<size_t> Matrix<T>::getSize() const{

    std::vector<size_t> result(2);

    result[0] = _size1;
    result[1] = _size2;

    return result;
};

template<typename T>
T& Matrix<T>::operator()(const size_t& r, const size_t& c){

    if (r >= _size1 || c >= _size2){

        throw std::out_of_range("Index is out of range!");
    };

    return _matrix[r][c];
};

template<typename T>
const T& Matrix<T>::operator()(const size_t& r, const size_t& c)const{

    if (r >= _size1 || c >= _size2){

        throw std::out_of_range("Index is out of range!");
    };

    return _matrix[r][c];
};

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& M)
{
    if (this == &M) return *this;

    // free old memory
    for (size_t i = 0; i < _size1; ++i){
        delete[] _matrix[i];
        allocations--;
    }
    delete[] _matrix;

    // allocate new
    _size1 = M._size1;
    _size2 = M._size2;
    _matrix = new T*[_size1];
    for (size_t i = 0; i < _size1; ++i) {
        _matrix[i] = new T[_size2];
        std::copy(M._matrix[i], M._matrix[i] + _size2, _matrix[i]);
        allocations++;
    }
    return *this;
};

template<typename T>
void Matrix<T>::printOut() const{

    std::cout << "[";
    for (size_t i = 0; i < _size1; ++i){
        if(i != 0) std::cout << " ";
        for (size_t j = 0; j < _size2; ++j){
            std::cout << _matrix[i][j];
           if(i != _size1-1 || j !=_size2-1) std::cout << ", ";
        }
        if (i != _size1-1){std::cout << '\n';};
    }
    std::cout << "]" << std::endl;
};

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& M2) const{
    if (_size1 != M2._size1 || _size2 != M2._size2){

            throw std::invalid_argument("Matrices have inequal sizes!");
    };

    Matrix<T> M3(_size1, _size2, T{0});
    for (size_t i = 0; i < _size1; ++i){
        for (size_t j = 0; j < _size2; ++j){

            M3(i,j) = (*this)(i,j) + M2(i,j);

        };
    };

    return M3;
};

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& M2) const{

    if (_size1 != M2._size1 || _size2 != M2._size2){

            throw std::invalid_argument("Matrices have inequal sizes! (operator-)");
    };

    Matrix<T> M3(_size1, _size2, T{0});

    for (size_t i = 0; i < _size1; ++i){
        for (size_t j = 0; j < _size2; ++j){

            M3(i,j) = (*this)(i,j) - M2(i,j);
        };
    };

    return M3;
};

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& M2) const{

    if (_size2 != M2._size1){
        throw std::runtime_error("Sizes don't match! (operator * )");
    };

    Matrix<T> result(_size1, M2._size2, T{0});

    for (size_t i = 0; i < result._size1; ++i){
        for (size_t j = 0; j < result._size2; ++j){

            T sum {0};
            for (size_t m = 0; m < _size2; ++m){

                sum +=  _matrix[i][m] * M2(m,j);
            };
            result(i,j)= sum;
        };
    };

    return result;
};

template<typename T>
Matrix<T> Matrix<T>::operator*(T scalar) const{

    Matrix<T> result(*this);
    for (size_t i = 0; i < _size1; ++i){
        for (size_t j = 0; j < _size2; ++j){
          result(i,j) = (*this)(i,j) * scalar;
        };
    };
    return result;
};

template<typename T>
void Matrix<T>::deleteRow(size_t r) {
    if (r >= _size1)
        throw std::out_of_range("Row index out of range! (deleteRow)");

    // Delete row
    delete[] _matrix[r];
    allocations--;

    // Shift remaining row pointers up
    for (size_t i = r; i < _size1 - 1; ++i)
        _matrix[i] = _matrix[i + 1];

    // Set the last pointer to nullptr
    _matrix[_size1 - 1] = nullptr;

    // Reduce the number of rows
    --_size1;
};

template<typename T>
void Matrix<T>::deleteRows(std::vector<size_t>& r){

    if (r.empty()){

        throw std::invalid_argument("Given rows to delete are missing! (deleteRows)");
    }

    std::sort(r.begin(), r.end(), std::greater<size_t>());

    if (r.size() > _size1){

        throw std::invalid_argument("There are more rows to be deleted than the row numbers in matrix! (deleteRows)");
    };

    if (r.back() >= _size1){

        throw std::invalid_argument("Row index out of bounds! (deleteRows)");
    };

    for (size_t i = 0; i < r.size(); ++i){

        deleteRow(r[i]);
    };
};

template<typename T>
void Matrix<T>::addRows(Matrix<T>& r){

    if (r.getSize()[1] != _size2){
      throw std::invalid_argument("Given row size do not match with original matrix! (addrows)");
    };

    size_t newSize = _size1+r.getSize()[0];
    Matrix<T> newMatrix(newSize,_size2);

    for (size_t i = 0; i < _size1; ++i){
        for (size_t j = 0; j < _size2; ++j){

            newMatrix(i,j) = (*this)(i,j);
        };
    };

    for (size_t k = _size1; k < newSize; ++k){
        for (size_t m = 0; m < r.getSize()[1]; ++m){
        newMatrix(k,m) = r(k-_size1,m);
        };
    };

    *this = newMatrix;
};


template<typename T>
void Matrix<T>::deleteColumn(size_t c){

    if (c >= _size2){
        throw std::out_of_range("Given index is out of range! (deleteColumn)");
    };

    for (size_t i = 0; i < _size1; ++i){

        T* newRow = new T[_size2 - 1];
        allocations++;

        for (size_t j = 0, nj = 0; j < _size2; ++j){

            if (j != c){

                newRow[nj++] = _matrix[i][j];
            };
        };
        delete[] _matrix[i];
        allocations--;
        _matrix[i] = newRow;
    };
    --_size2;
};

template<typename T>
void Matrix<T>::deleteColumns(std::vector<size_t>& c){
    if (c.empty()){

        throw std::invalid_argument("Given rows to delete are missing! (deleteColumns)");
    }

    std::sort(c.begin(), c.end(), std::greater<size_t>());

    if (c.size() > _size2){

        throw std::invalid_argument("There are more rows to be deleted than the row numbers in matrix! (deleteColumns)");
    };

    if (c.back() >= _size2){

        throw std::invalid_argument("Row index out of bounds! (deleteColumns)");
    };

    for (size_t i = 0; i < c.size(); ++i){

        deleteColumn(c[i]);
    };
};

template<typename T>
std::vector<T> Matrix<T>::mVm(const std::vector<T>& vec) const{

    if (_size2 != vec.size()){

      throw std::invalid_argument("Matrix-vector sizes don't match! (mVm)");
    };

    std::vector<T> result(_size1,T{0});

    for (size_t i = 0; i < _size1; ++i){
        T sum {0};
        for (size_t j = 0; j < _size2; ++j){

         sum += _matrix[i][j] * vec[j];
        };
        result[i] = sum;
    };

    return result;
};

template<typename T>
Matrix<T>  Matrix<T>::transpose() const{

    Matrix<T> t(_size2, _size1, T{0});

    for (size_t i = 0; i < _size2; ++i){
        for (size_t j = 0; j < _size1; ++j){
            t(i,j) = _matrix[j][i];
        };
    };

    return t;
};

template<typename T>
Matrix<T> Matrix<T>::cho() const{

    if (_size1 != _size2){
        throw std::invalid_argument("Given matrix is not square! (cho)");
    };

    Matrix<T> L(_size1,_size2,T{0});

    for (size_t i = 0; i < _size1; ++i){
        for (size_t j = 0; j <= i; ++j){
            T sum{0};
            for (size_t k = 0; k < j; ++k){
                sum += L(j,k)*L(i,k);
            };
               if(i==j){
                   L(j,j) = std::sqrt(_matrix[j][j]-sum);
               }
               else{
                 L(i,j) = (_matrix[i][j]-sum)/L(j,j);
               };

        };
    };

    return L;
};

template<typename T>
Matrix<T> Matrix<T>::L_inverse() const {

    Matrix<T> L = *this;
    if (L.getSize()[0] != L.getSize()[1]){
      throw std::invalid_argument("Given triangular matrix is not square! (Matrix::L_inverse)");
    }
    for (size_t i = 0; i < L.getSize()[0]; ++i){
        for (size_t j = 0; j < L.getSize()[1]; ++j){
          if (j > i){
            if (L(i,j) != 0){
                throw std::invalid_argument("Given matrix is not triangular! (Matrix::L_inverse)");
            };
          };
        };
    };
    Matrix<T> M(L.getSize()[0], L.getSize()[1], T{0});

    T sum;
    for (size_t j = 0; j < L.getSize()[0]; ++j){
        M(j,j) = T{1}/L(j,j);
        for (size_t i = j+1; i < L.getSize()[0]; ++i){
         sum = {0};
         for(size_t k = j; k < i; ++k){

             sum += L(i,k)*M(k,j);
         };

         M(i,j) = -sum/L(i,i);
        };
    };

    return M;
};

#endif
