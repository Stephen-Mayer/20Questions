#include "Matrix.h"

Matrix::Matrix(){}

Matrix::Matrix(int rows, int cols)
{
    for(int i = 0; i < rows; i++)
    {
        vector<double> col(cols, 0);
        matrix.push_back(col);
    }
}

Matrix::Matrix(int rows, int cols, double value)
{
    for(int i = 0; i < rows; i++)
    {
        vector<double> col(cols, value);
        matrix.push_back(col);
    }
}

Matrix::Matrix(const vector<vector<double>> &values)
{
    for(vector<double> vector : values)
    {
        matrix.push_back(vector);
    }
}

int Matrix::getRows() const
{
    return matrix.size();
}

int Matrix::getCols() const
{
    return matrix[0].size();
}

int Matrix::getRowIndexOfMaxValue() const
{
    double maxValue = matrix[0][0];
    int rowIndex = 0;
    for(int i = 0; i < getRows(); i++)
    {
        for(int j = 0; j < getCols(); j++)
        {
            if(matrix[i][j] > maxValue)
            {
                maxValue = matrix[i][j];
                rowIndex = i;
            }
        }
    }
    return rowIndex;
}

const vector<double>& Matrix::operator[](int row) const
{
    return matrix[row];
}

vector<double>& Matrix::operator[](int row)
{
    return matrix[row];
}

Matrix Matrix::operator*(const Matrix& other) const
{
    if(getCols() != other.getRows())
    {
        throw invalid_argument("Cannot mulptiply matrices due to invalid sizes");
    }
    int rows = getRows();
    int cols = other.getCols();
    Matrix result(rows, cols);

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            for(int k = 0; k < getCols(); k++)
            {
                result[i][j] += matrix[i][k] * other[k][j];
            } 
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix& other) const 
{
    if (getRows() != other.getRows() || getCols() != other.getCols()) 
    {
        throw invalid_argument("Cannot add matrices due to invalid sizes");
    }
    Matrix result(getRows(), getCols());

    for (int i = 0; i < getRows(); i++) 
    {
        for (int j = 0; j < getCols(); j++) 
        {
            result[i][j] = (*this)[i][j] + other[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const 
{
    if (getRows() != other.getRows() || getCols() != other.getCols()) 
    {
        throw invalid_argument("Cannot subtract matrices due to invalid sizes");
    }
    Matrix result(getRows(), getCols());

    for (int i = 0; i < getRows(); i++) 
    {
        for (int j = 0; j < getCols(); j++) 
        {
            result[i][j] = (*this)[i][j] - other[i][j];
        }
    }
    return result;
}

Matrix& Matrix::setMatrixEuqal(const Matrix &other)
{
    if (getRows() != other.getRows() || getCols() != other.getCols()) 
    {
        throw invalid_argument("Cannot assign matrices due to invalid sizes");
    }
    if (this != &other) 
    {
        for (int i = 0; i < getRows(); i++) 
        {
            for (int j = 0; j < getCols(); j++) 
            {
                if(matrix[i][j] != other[i][j])
                {
                    matrix[i][j] = other[i][j];
                }
            }
        }
    }
    return *this;
}

Matrix Matrix::elementwiseMultiplication(const Matrix &other) const
{
    if (getRows() != other.getRows() || getCols() != other.getCols()) 
    {
        throw invalid_argument("Cannot element multiply matrices due to invalid sizes");
    }   
    Matrix result(getRows(), getCols());
    for (int i = 0; i < getRows(); i++) 
    {
        for (int j = 0; j < getCols(); j++) 
        {
            result[i][j] = (*this)[i][j] * other[i][j];
        }
    }
    return result;
}

Matrix Matrix::scalarMultiplication(double scalar) const
{
    Matrix result(getRows(), getCols());
    for (int i = 0; i < getRows(); i++) 
    {
        for (int j = 0; j < getCols(); j++) 
        {
            result[i][j] = (*this)[i][j] * scalar;
        }
    }
    return result;
}

Matrix Matrix::reluActivation() const
{
    Matrix result = *this;
    for(vector<double>& row : result.matrix)
    {
        for(double &val : row)
        {
            val = max(0.0, val);
        }
    }
    return result;
}

Matrix Matrix::transposeMatrix() const
{
    Matrix transposedMatrix(getCols(), getRows());
    for (int i = 0; i < getRows(); ++i) 
    {
        for (int j = 0; j < getCols(); ++j) 
        {
            transposedMatrix[j][i] = matrix[i][j]; 
        }
    }
    return transposedMatrix;
}

void Matrix::print() const
{
    for(vector<double> vector : matrix)
    {
        for(double value : vector)
        {
            cout << value << " ";
        }
        cout << endl;
    }
}