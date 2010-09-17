#ifndef BALL_H
#define BALL_H

class Jet : public PhyObj, NetObj
{
	private:
		::BITMAP *_bmp;
		int _col;

	public:
		// if addr is NULL, assumed to be localhost
		Jet(double x, double y, double speed, double heading,
				::BITMAP *bmp, int col,
				struct ::sockaddr_in *addr);

		Jet(double w, double h, double speed,
				BITMAP *bmp, struct sockaddr_in *addr);
		// everything else randomise

		~Jet();

		void draw(::BITMAP *);

		inline BITMAP *bmp() { return _bmp; }
		inline int     col() { return _col; }
};

#endif
