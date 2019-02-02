#pragma once
#include "GeometricObject.h"
#include "Point.h"
namespace Object {
	class Surface :
		public GeometricObject
	{
	public:
		Vector3 normal;
		Point center;
		double perimeter;
		double area;

		Surface()
		{
			GeometricType = SURFACE;
		}
		~Surface() {

		}

		/// <summary>
		/// Create polygonal mesh
		/// Must be overloaded
		/// </summary>
		void CreateMesh() {};//TODO pure virtual

		virtual void CalculatePerimeter() = 0;
		virtual void CalculateArea() = 0;
	};

}