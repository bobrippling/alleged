#ifndef PHYOBJ_H
#define PHYOBJ_H

class PhyObj
{
	protected:
		double _x, _y,
					 _speed, _heading,
					 _w, _h;

		inline void getcomponents(double &, double &);
		inline void setcomponents(double  , double  );

	public:
		PhyObj(double x, double y,
				double speed, double heading,
				double w, double h);
		virtual ~PhyObj();

		void move(double xlim, double ylim);

		/* accessor */
		inline double x() { return _x; }
		inline double y() { return _y; }
};

#endif
