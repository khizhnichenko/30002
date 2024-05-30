#include "InputFormatters.h"

std::istream& operator>>(std::istream& in, DelimiterI&& dest)
{
  char c;
  in >> c;
  if (in && c != dest.exp)
    in.setstate(std::ios::failbit);
  return in;
}

std::istream& operator>>(std::istream& in, LongLongI&& dest)
{
  return in >> dest.num >> DelimiterI{ 'u' } >> DelimiterI{ 'l' } >> DelimiterI{ 'l' };
}

std::istream& operator>>(std::istream& in, StringI&& dest)
{
  return std::getline(in >> DelimiterI{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, LabelI&& dest)
{
  return std::getline(in, dest.ref, ' ');
}

std::istream& operator>>(std::istream& in, ComplexI&& dest)
{
  char ch;
  if (in >> ch && ch == '#')
  {
    if (in >> ch && ch == 'c')
    {
      if (in >> ch && ch == '(')
      {
        double real, imag;
        if (in >> real >> imag >> ch && ch == ')')
        {
          dest.ref = std::complex<double>(real, imag);
          return in;
        }
      }
    }
  }
  in.setstate(std::ios::failbit);
  return in;
}
