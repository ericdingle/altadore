#include "altadore/ray_tracer/ray_tracer.h"
#include "altadore/scene/shape_node.h"
#include "altadore/shape/cube.h"
#include "altadore/shape/sphere.h"
#include "altadore/visual/material.h"
#include "bonavista/memory/scoped_refptr.h"

int main() {
  memory::scoped_refptr<TransformNode> root(new TransformNode());
  root->Translate(0, 0, -5);

  memory::scoped_refptr<ShapeNode> sphere1(new ShapeNode(new Sphere(), new Material(new Color(1.0, 0.0, 1.0), 25, 0.25)));
  root->AddChild(sphere1.ptr());

  memory::scoped_refptr<ShapeNode> sphere2(new ShapeNode(new Cube(), new Material(new Color(0.0, 1.0, 0.0), 15, 0)));
  memory::scoped_refptr<TransformNode> transform(new TransformNode());
  transform->Scale(0.2);
  transform->Translate(-1.5, 1.5, 1.5);
  transform->AddChild(sphere2.ptr());
  root->AddChild(transform.ptr());

  memory::scoped_refptr<LightVector> lights(new LightVector());
  lights->AddLight(new Light(new Point3(-5, 5, 0), new Color(1.0, 1.0, 1.0)));

  root->CalculateTransforms(Matrix4());
  RayTracer ray_tracer(root.ptr(), lights.ptr());
  if (!ray_tracer.Render("test.bmp", 600, 400, false))
    printf("Could not render!");

  return 0;
}
