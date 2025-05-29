#include "util.h"
#include <QDebug>
//#include <QtGlobal>

QString Util::path = "";

void Util::Tokenize(const std::string& str,
                      std::vector<std::string>& tokens,
                      const std::string& delimiters)
{
  // Skip delimiters at beginning.
  std::string s = str;
  int wn= s.find(13);
  if (wn!=-1) s.erase(wn,1);
  
  std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos     = s.find_first_of(delimiters, lastPos);
  
  while (std::string::npos != pos || std::string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      tokens.push_back(s.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = s.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = s.find_first_of(delimiters, lastPos);
    }
  
}	


  
const char* Util::read_textfile(std::string filename) {
  std::ifstream f(filename.c_str(), std::ios::in);
  std::string cnt, sum;
  sum = "";
  while(!f.eof()) {
    f >> cnt; 
    sum = sum + cnt;
  }
  f.close();       
  return sum.c_str();           
}


void Util::verify_file(std::string filename) {
  std::ifstream f(filename.c_str(), std::ios::in | std::ios::binary);
  if (!f.is_open())
    throw std::string("Unable to find file: " + filename);
  f.close();
}
bool Util::verify_file_bool(std::string filename) {
  std::ifstream f(filename.c_str(), std::ios::in | std::ios::binary);
  if (!f.is_open())
    return false;
  f.close();
  return true;
}

using namespace std;

string Util::trim(string strin)
{
  string str = strin;
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
  return str;
}    

double Util::clamp(double val, const double mi, const double ma) {
    val = min(ma, val);
    val = max(mi, val);
    return val;
}

QVector3D Util::clamp(const QVector3D val, const double min, const double max)
{
    QVector3D v = val;
    v.setX(clamp(val.x(),min,max));
    v.setY(clamp(val.y(),min,max));
    v.setZ(clamp(val.z(),min,max));
    return v;
}

QString Util::getFileName(QString dir, QString baseName, QString type)
{
    QDirIterator it(dir,
                    QStringList() << "*.*", QDir::Files, QDirIterator::Subdirectories);
    int maxNumber = 1;
    while (it.hasNext()) {
        QString filename = it.next();
        QStringList split = filename.split("/");
        filename = split[split.length()-1];
        if (filename.contains(baseName)) {
            filename = filename.replace(baseName, "");
            filename = filename.replace("."+type, "");
            int num = filename.toInt();
            maxNumber = max(maxNumber, num);
        }
    }
    maxNumber++;
    return baseName + QString::number(maxNumber).rightJustified(4, '0');// + "." + type;

}



double Util::smoothstep(double edge0, double edge1, double x)
{
    // Scale, bias and saturate x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*(3 - 2 * x);
}

