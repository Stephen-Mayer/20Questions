#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include <new>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

class Matrix
{
    public:
        Matrix();
        Matrix(int rows, int cols); //initializes an mxn matrix
        Matrix(int rows, int cols, double value);
        Matrix(const vector<vector<double>> &values);
        int getRows() const; //returns m
        int getCols() const; //returns n
        int getRowIndexOfMaxValue() const;
        const vector<double>& operator[](int row) const;
        vector<double>& operator[](int row);
        Matrix operator*(const Matrix& other) const;
        Matrix operator+(const Matrix &other) const;
        Matrix operator-(const Matrix &other) const;
        Matrix& setMatrixEuqal(const Matrix &other);
        Matrix elementwiseMultiplication(const Matrix &other) const;
        Matrix scalarMultiplication(double scalar) const;
        Matrix reluActivation() const;
        Matrix transposeMatrix() const;
        void print() const;

    private:
        vector<vector<double>> matrix;
};

#endif