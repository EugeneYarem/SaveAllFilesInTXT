#include <iostream>
#include <iterator>
#include <fstream>
#include <QCoreApplication>
#include <QDir>
#include <string>

using namespace std;

void writeConcreteFile(ofstream & out, const string & file)
{
    ifstream in(file, std::ofstream::in);
    string str;
    while(getline(in, str, '\n'))
        if(!QRegExp("\\s*").exactMatch(QString::fromStdString(str)))
            out << str << "\n";
    in.close();
}

void writeFilesFromDir(ofstream & out, const QDir & dir, const QString & dirName)
{
    cout << endl;
    out << "\n\t\t\t" << dir.dirName().toStdString() << "\n";
    QFileInfoList list = dir.entryInfoList();
    int dirNumber = 0, fileNumber = 0;
    for(auto i : list)
    {
        cout << i.absoluteFilePath().toStdString() << endl;
        if(i.isDir())
        {
            QDir newDir = QDir(i.absoluteFilePath());
            newDir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
            QString dN = dirName;
            if(!i.dir().dirName().contains("laba"))
                dN += i.dir().dirName() + "/";
            writeFilesFromDir(out, newDir, dN);
            ++dirNumber;
        }
        else
        {
            if(i.baseName() != "main" && (i.completeSuffix() == "cpp" || i.completeSuffix() == "h"))
            {
                if(!i.dir().dirName().contains("laba"))
                    out << "\n\t\t\t" << dirName.toStdString() + i.dir().dirName().toStdString() << "/" << i.fileName().toStdString() << "\n\n";
                else out << "\n\t\t\t" << i.fileName().toStdString() << "\n\n";

                writeConcreteFile(out, i.absoluteFilePath().toStdString());
                ++fileNumber;
            }
        }
    }
    cout << "\t" << dir.path().toStdString() << " has " << list.size() << " elements: " << dirNumber << " directories, " << fileNumber << " files" << endl << endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc > 1)
    {
        QDir dir(QString::fromLatin1(argv[1]));
        if(!dir.exists())
        {
            cout << "Wrong directory path" << endl;
            return EXIT_FAILURE;
        }

        dir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

        string outFileName;
        if(argc > 2)
        {
            string checkOutName = string(argv[2]);
            checkOutName.erase(std::remove(checkOutName.begin(), checkOutName.end(), ' '), checkOutName.end());
            if(checkOutName.size() == 0)
                outFileName = string(argv[1]).append("_out.txt");
            else {
                outFileName = string(argv[2]);
                if(outFileName.find('.') == string::npos)
                    outFileName.append(".txt");
            }
            if(outFileName.find('\\') == string::npos)
            {
                string temp(argv[1]);
                outFileName = temp.erase(temp.find_last_of('\\'), temp.size() - 1) + "\\" + outFileName;
            }
        }
        else
            outFileName = string(argv[1]).append("_out.txt");

        cout << endl << "Output file's name: " << outFileName << endl << endl;

        ofstream out(outFileName, std::ofstream::out);

        writeFilesFromDir(out, dir, "");
        out << "\n\t\t\t" << "main.cpp" << "\n\n";
        writeConcreteFile(out, dir.absoluteFilePath("main.cpp").toStdString());

        out.close();
        return EXIT_SUCCESS;
    }
    else
    {
        cout << "Directory path can not be empty" << endl;
        return EXIT_FAILURE;
    }

    return a.exec();
}
