#ifndef PHYOBJ_H
#define PHYOBJ_H

class PhyObj
{
	protected:
		double _x, _y,
					 _speed, _heading,
					 _w, _h;

	public:
		PhyObj(double x, double y,
				double speed, double heading,
				double w, double h);
		virtual ~PhyObj();

		virtual void draw(BITMAP *buffer) const = 0; // boom, virtual ,abstract class
		virtual void move(double xlim, double ylim);
};

#endif
