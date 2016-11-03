//
// Created by vineet on 2/11/16.
//

#include <iostream>
#include "data_frame.h"

namespace utils {
    
    namespace data {

        data_frame::data_frame(int ncols) {
            _ncols = ncols;
            _data = std::vector<std::vector<double> >(_ncols, std::vector<double>(0));
        }

        void data_frame::initialise_colnames(int ncols) {
            if (ncols < 0) {
                std::cerr << __PRETTY_FUNCTION__ << " : Invalid number of columns" << std::endl;
                return;
            }
            std::vector<std::string> colnames;
            for(int i=0; i<ncols; i++) {
                colnames.push_back("C" + std::to_string(i));
            }
            initialise_colnames(colnames);
        }

        void data_frame::initialise_colnames(std::vector<std::string>& colnames) {
            _colnames = colnames;
            // Col number start from 0
            for(int i=0; i<colnames.size(); i++) {
                _colnames_index[colnames[i]] = i;
            }
        }

        bool data_frame::remove_row(int index) {
            if (index < 0 || index >= get_size()) {
                std::cerr << __PRETTY_FUNCTION__ << " : Index " << index
                          << " out of bound"<< std::endl;
                return false;
            }
            for(int i=0; i<_ncols; i++) {
                _data[i].erase(_data[i].begin() + index);
            }
            return true;
        }

        bool data_frame::insert_row(data_row dr) {
            if (dr.get_size() != _ncols) {
                std::cerr << __PRETTY_FUNCTION__ << " : Number of columns in data row( "
                          << dr.get_size() << ") does not match number of columns in data frame("
                          << _ncols  << ")"<< std::endl;
                return false;
            }
            for(int i=0; i<_ncols; i++) {
                _data[i].push_back(dr[i]);
            }
            return true;
        }

        bool data_frame::insert_row(std::vector<double>& row) {
            if (row.size() != _ncols) {
                std::cerr << __PRETTY_FUNCTION__ << " : Number of columns in data row( "
                          << row.size() << ") does not match number of columns in data frame("
                          << _ncols << ")" << std::endl;
                return false;
            }
            for(int i=0; i<_ncols; i++) {
                _data[i].push_back(row[i]);
            }
        }

        bool data_frame::insert_column(std::vector<double>& col) {
            return insert_column(col, "C" + std::to_string(_ncols+1));
        }

        bool data_frame::insert_column(std::vector<double>& col, std::string colname) {
            if (get_size() != col.size()) {
                std::cerr << __PRETTY_FUNCTION__ << " : Number of rows in data column( "
                          << col.size() << ") does not match number of rows in data frame(" << get_size()
                          << ")" << std::endl;
                return false;
            }
            _colnames.push_back(colname);
            _colnames_index[colname] = _ncols;
            _ncols++;
            _data.push_back(col);
        }

        bool data_frame::remove_column(int index) {
            if (index < 0 || index >= _ncols) {
                std::cerr << __PRETTY_FUNCTION__ << " : Index " << index
                          << " out of bound"<< std::endl;
                return false;
            }
            _ncols--;
            std::string colname = _colnames[index];
            _colnames.erase(_colnames.begin() + index);
            _colnames_index.erase(colname);
            _data.erase(_data.begin() + index);
        }

        bool data_frame::remove_column(const std::string& colname) {
            if (_colnames_index.find(colname) == _colnames_index.end()) {
                std::cerr << __PRETTY_FUNCTION__ << " : Invalid column name specified("
                          << colname << ")"<< std::endl;
            }
            return remove_column(_colnames_index[colname]);
        }

        data_row data_frame::operator[](int index) {
            std::vector<double> data;
            if (index < 0 || index >= get_size()) {
                std::cerr << __PRETTY_FUNCTION__ << " : Index " << index
                          << " out of bound"<< std::endl;
                std::vector<std::string> empty_colnames;
                return data_row(data, empty_colnames);
            }

            for(int i=0; i<_ncols; i++) {
                data.push_back(_data[i][index]);
            }
            return data_row(data, _colnames);
        }

        data_row data_frame::operator[](const std::string& colname) {
            if (_colnames_index.find(colname) == _colnames_index.end()) {
                std::cerr << __PRETTY_FUNCTION__ << " : Invalid column name specified("
                          << colname << ")"<< std::endl;
                return operator[](-1);
            }
            int index = _colnames_index[colname];
            return operator[](index);
        }

        int data_frame::get_size() {
            if (_ncols == 0) return 0;
            return _data[0].size();
        }

        void data_frame::print(int rows) {
            if (rows == -1) rows = get_size();
            else if (rows > get_size()) rows = get_size();
            for(int i=0; i<_ncols; i++) {
                std::cout << _colnames[i] << " ";
            }
            std::cout << std::endl;
            for(int i=0; i<rows; i++) {
                for(int j=0; j<_ncols ; j++) {
                    std::cout << (*this)[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }

        void data_frame::print_col_names() {
            for(int i=0; i<_colnames.size(); i++) {
                std::cout << _colnames[i] << " " ;
            }
            std::cout << std::endl;
        }

    } // end namespace data
} // end namespace utils

