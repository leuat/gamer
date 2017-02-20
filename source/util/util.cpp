#include "util.h"
#include <QDebug>
//#include <QtGlobal>

QString Util::path = "";

void Util::Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters)
{
  // Skip delimiters at beginning.
  string s = str;
  int wn= s.find(13);
  if (wn!=-1) s.erase(wn,1);
  
  string::size_type lastPos = s.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos     = s.find_first_of(delimiters, lastPos);
  
  while (string::npos != pos || string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      tokens.push_back(s.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = s.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = s.find_first_of(delimiters, lastPos);
    }
  
}	


  
const char* Util::read_textfile(string filename) {
  ifstream f(filename.c_str(), ios::in);
  string cnt, sum;
  sum = "";
  while(!f.eof()) {
    f >> cnt; 
    sum = sum + cnt;
  }
  f.close();       
  return sum.c_str();           
}


void Util::verify_file(string filename) {
  ifstream f(filename.c_str(), ios::in | ios::binary);
  if (!f.is_open())
    throw string("Unable to find file: " + filename);
  f.close();
}
bool Util::verify_file_bool(string filename) {
  ifstream f(filename.c_str(), ios::in | ios::binary);
  if (!f.is_open())
    return false;
  f.close();
  return true;
}


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

float Util::clamp(float val, float mi, float ma) {
    val = min(ma, val);
    val = max(mi, val);
    return val;
}

QVector3D Util::clamp(QVector3D val, float min, float max)
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



float Util::smoothstep(float edge0, float edge1, float x)
{
    // Scale, bias and saturate x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*(3 - 2 * x);
}

