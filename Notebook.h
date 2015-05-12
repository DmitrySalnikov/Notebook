#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

using std::size_t;
using std::string;
using std::strlen;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

class Notebook {
private:
    static size_t const LENGTH = 256;
    struct Vertex { 
        Vertex * vertex; 
        char * phone; 
        Vertex() : vertex(nullptr), phone(nullptr) {}
    };
    Vertex * head;

    void view(Vertex const * vert, string & name, bool & isempty) const {
        if (!vert)
            return;

        for (size_t i{0}; i < LENGTH; ++i) {
            name.push_back(char(i));

            if (vert[i].phone) {
                cout << name << ": " << vert[i].phone << endl;
                isempty = false;
            }
            
            view(vert[i].vertex, name, isempty);
            
            name.pop_back();
        }
    }
    
    void import(Vertex const * vert, string & name, char const * filename, ofstream & fout) const {
        if (!vert)
            return;

        for (size_t i{0}; i < LENGTH; ++i) {
            name.push_back(char(i));

            if (vert[i].phone) 
                fout << name << endl << vert[i].phone << endl;
            
            import(vert[i].vertex, name, filename, fout);
            
            name.pop_back();
        }
    }

    void destr(Vertex const * vert) {
        if (!vert)
            return;

        for (size_t i{0}; i < LENGTH; ++i) {
            destr(vert[i].vertex);

            delete [] vert[i].phone;
            delete [] vert[i].vertex;
        }
    }

    Notebook(Notebook const & a) {} 

    Notebook const & operator=(Notebook const & a) const { return *this; }
public:
    Notebook() : head(new Vertex [LENGTH]) {}

    ~Notebook() {
        destr(head);
        delete [] head;
    }    

    void add(char const * name, char const * phone) {
        size_t n = strlen(name);

        Vertex * temp = head;
        
        for (size_t i{0}; i < n; ++i) {
            if (!temp[name[i]].vertex)
                temp[name[i]].vertex = new Vertex [LENGTH];

            temp = temp[name[i]].vertex;
        }

        delete [] temp[0].phone;
        temp[0].phone = new char [strlen(phone) + 1];
        strcpy(temp[0].phone, phone);
    }

    char const * find(char const * name) {
        size_t n = strlen(name);

        Vertex * temp = head;
                
        for (size_t i{0}; i < n; ++i) {
            if (!temp[name[i]].vertex)
                return nullptr;

            temp = temp[name[i]].vertex;
        }

        return temp[0].phone;
    }

    void remove(char const * name) {
        size_t n = strlen(name);
        
        Vertex * temp = head;
                
        for (size_t i{0}; i < n; ++i) {
            if (!temp[name[i]].vertex)
                return;

            temp = temp[name[i]].vertex;
        }

        delete [] temp[0].phone;
        temp[0].phone = nullptr;
    }

    void show_all() const {
        string name;
        bool isempty = true;
        view(head, name, isempty);
        if (isempty)
            cout << "Notebook is empty" << endl;
    }

    void export_notes(char const * filename) {
         ifstream fin(filename);
         char name [100] = {};
         fin.getline(name, 99);
         char phone [100] = {};
         fin.getline(phone, 99);
         while (fin) {
             add(name, phone);
             fin.getline(name, 99);
             fin.getline(phone, 99);
         } 
         fin.close();
    }

    void import_notes(char const * filename) {
        ofstream fout(filename);

        string name;
        import(head, name, filename, fout);

        fout.close();
    }
};
