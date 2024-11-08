#include <algorithm>
#include <functional>
#include <iomanip>
#include <numeric>
#include <string>

#include "Commands.h"
#include "Functors.h"
#include "IOHelpers.h"
#include "Predicates.h"

namespace khizhnichenko {
  void area(const std::vector< Polygon >& data, std::istream& in, std::ostream& out) {
    khizhnichenko::iofmtguard guard(out);
    using namespace std::placeholders;
    std::string arg;
    in >> arg;
    std::function< double(double, const Polygon& rhs) > func;

    if (arg == "MEAN") {
      if (data.empty()) {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      func = std::bind(detail::accAreaMean, _1, _2, data.size());
    }
    else {
      predicate_t pred;
      if (arg == "ODD") {
        pred = hasOddVertexesCount;
      }
      else if (arg == "EVEN") {
        pred = hasEvenVertexesCount;
      }
      else {
        int size = std::stoul(arg);
        if (size < 3) {
          throw std::invalid_argument("<INVALID COMMAND>");
        }
        pred = std::bind(checkVertexesCount, _1, size);
      }
      func = std::bind(detail::accAreaIf, _1, _2, pred);
    }
    out << std::fixed << std::setprecision(1);
    out << std::accumulate(data.cbegin(), data.cend(), 0.0, func);
  }

  void min(const std::vector<Polygon>& data, std::istream& in, std::ostream& out) {
    if (data.empty()) {
      throw std::invalid_argument("INVALID COMMAND");
    }

    khizhnichenko::iofmtguard guard(out);
    std::string cmd;
    in >> cmd;
    if (cmd == "AREA") {
      out << std::fixed << std::setprecision(1);
      out << std::min_element(data.cbegin(), data.cend(), isLessArea)->computeArea();
    }
    else if (cmd == "VERTEXES") {
      out << std::min_element(data.cbegin(), data.cend(), isLessVertexesCount)->points.size();
    }
  }

  void max(const std::vector<Polygon>& data, std::istream& in, std::ostream& out) {
    if (data.empty()) {
      throw std::invalid_argument("INVALID COMMAND");
    }

    khizhnichenko::iofmtguard guard(out);
    std::string cmd;
    in >> cmd;
    if (cmd == "AREA") {
      out << std::fixed << std::setprecision(1);
      out << std::max_element(data.cbegin(), data.cend(), isLessArea)->computeArea();
    }
    else if (cmd == "VERTEXES") {
      out << std::max_element(data.cbegin(), data.cend(), isLessVertexesCount)->points.size();
    }
  }

  void count(const std::vector<Polygon>& data, std::istream& in, std::ostream& out) {
    iofmtguard guard(out);
    using namespace std::placeholders;
    std::string arg;
    in >> arg;
    predicate_t pred;
    if (arg == "ODD") {
      pred = hasOddVertexesCount;
    }
    else if (arg == "EVEN") {
      pred = hasEvenVertexesCount;
    }
    else {
      int size = std::stoul(arg);
      if (size < 3) {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      pred = std::bind(checkVertexesCount, _1, size);
    }
    std::cout << std::count_if(data.cbegin(), data.cend(), pred);
  }

  void rects(std::istream &in, std::ostream &out) {
    std::vector<Polygon> data;
    Polygon polygon;

    while (in >> polygon) {
      data.push_back(polygon);
    }

    if (data.empty()) {
      throw std::logic_error("<INVALID COMMAND>");
    }

    auto rectPredicate = [](const Polygon &polygon)
    { return isRectangle(polygon); };
    out << std::count_if(data.cbegin(), data.cend(), rectPredicate) << std::endl;
  }

  void same(const std::vector<Polygon>& data, std::istream& in, std::ostream& out) {
    if (data.empty()) {
      throw std::invalid_argument("INVALID COMMAND");
    }

    Polygon target;
    in >> target;

    if (!in || in.peek() != '\n') {
      throw std::invalid_argument("<INVALID COMMAND>");
    } else {
      std::sort(target.points.begin(), target.points.end());
      auto countFunc = [&target](const Polygon &polygon) {
        return polygon.isSamePolygon(target);
      };
      out << std::count_if(data.begin(), data.end(), countFunc);
    }
  }

  double detail::accAreaIf(double val, const Polygon& polygon, predicate_t pred) {
    return val + polygon.computeArea() * pred(polygon);
  }

  double detail::accAreaMean(double val, const Polygon& polygon, std::size_t size) {
    return val + polygon.computeArea() / size;
  }
}