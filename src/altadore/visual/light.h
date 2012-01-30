#ifndef ALTADORE_VISUAL_LIGHT_H_
#define ALTADORE_VISUAL_LIGHT_H_

#include "altadore/algebra/point3.h"
#include "altadore/visual/color.h"
#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_refptr.h"

class Light : public memory::RefCount {
public:
  Light(const Point3* position, const Color* color);
  ~Light();

  const Point3* position() const { return position_.ptr(); }
  const Color* color() const { return color_.ptr(); }

private:
  Light(const Light&);
  void operator=(const Light&);

  memory::scoped_refptr<const Point3> position_;
  memory::scoped_refptr<const Color> color_;
};

#endif
