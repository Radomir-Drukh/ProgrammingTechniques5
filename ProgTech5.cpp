#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include <map>
#include <deque>
#include <random>

using namespace std;

string outputFileName = "report";
string inputFile = "data.txt";

struct Student {
    string name;
    string faculty;
    int group;
    double average;

    Student(string name, string faculty, int group, double average)
    {
        this->name = name;
        this->faculty = faculty;
        this->group = group;
        this->average = average;
    }

    friend ostream& operator<<(ostream& os, const Student& a) {
        os << a.average << ' ' << a.name << ' ' << a.faculty << '\n';
        return os;
    }
};

class Report {
    vector<Student> data;
public:
    void loadData() {
        ifstream fin(inputFile);

        int dim;
        string name, faculty;
        int group;
        double average;
 
        fin >> dim;
        for (int j = 0; j < dim; ++j) {
            fin >> name >> faculty >> group >> average;
            Student temp(name, faculty, group, average);
            data.push_back(temp);
        }
    };

    vector<Student> getData() {
        return this->data;
    }
};

Report report;

struct Writer {
    virtual void writeReport() const = 0;
};

class TxtWriter : public Writer {
public:
    void writeReport() const override {
        ofstream fout(outputFileName + ".txt");
        vector<Student> data = report.getData();

        for (int i = 0; i < data.size(); ++i)
            fout << "Средний балл студента " << data[i].name << " из " << data[i].group << " группы "
             << " факультета " << data[i].faculty << " - " << data[i].average << "\n";
    }
};

class CsvWriter : public Writer {
public:
    void writeReport() const override {
        ofstream fout(outputFileName + ".csv");
        fout << "Имя студента;группа;факультет;средний балл\n";

        vector<Student> data = report.getData();

        for (int i = 0; i < data.size(); ++i)
            fout << data[i].name + ";" + to_string(data[i].group) + ";"
            + data[i].faculty + ";" + to_string(data[i].average) + "\n";

    }
};

class XmlWriter : public Writer {
public:
    void writeReport() const override {
        ofstream fout(outputFileName + ".xml");

        fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        fout << "<systemReport>\n";

        vector<Student> data = report.getData();

        for (int i = 0; i < data.size(); ++i) {
            fout << "\t<student>\n";
            fout << "\t\t<name>" << data[i].name << "</name>\n";
            fout << "\t\t<group>" << data[i].group << "</group>\n";
            fout << "\t\t<faculty>" << data[i].faculty << "</faculty>\n";
            fout << "\t\t<average>" << data[i].average << "</average>\n";
            fout << "\t</student>\n";
        }

        fout << "</systemReport>\n";
    }
};

class WriterCreator {
public:
    virtual Writer* createWriter() const = 0;
};

class TxtWriterCreator : public WriterCreator {
public:
    Writer* createWriter() const override {
        return new TxtWriter();
    };
};

class CsvWriterCreator : public WriterCreator {
public:
    Writer* createWriter() const override {
        return new CsvWriter();
    };
};

class XmlWriterCreator : public WriterCreator {
public:
    Writer* createWriter() const override {
        return new XmlWriter();
    };
};
int main()
{
    setlocale(LC_ALL, "Russian");
    report.loadData();

    int repeat = 1;

    while (repeat) {
        WriterCreator* creator; 
        Writer* writer;

        cout << "\nВыберите режим работы: 1 - txt, 2 - csv, 3 - xml\n";
        int mode = 1;
        cin >> mode;

        if (mode == 1) {
            creator = new TxtWriterCreator();
            writer = creator->createWriter();
        } else
        if (mode == 2) {
            creator = new CsvWriterCreator();
            writer = creator->createWriter();
        } else {
            creator = new XmlWriterCreator();
            writer = creator->createWriter();
        }

        writer->writeReport();

        cout << "\nПовторить? 1 - да, 0 - нет\n";
        cin >> repeat;
    }

    return 0;
}
