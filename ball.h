#ifndef BALL_H
#define BALL_H

class Ball : public PhyObj
{
	private:
		BITMAP *_bmp;
		int _col;

	public:
		Ball(double x, double y,
				double speed, double heading,
				BITMAP *bmp, int col);
		~Ball();

		inline BITMAP *bmp() { return _bmp; }
		inline int     col() { return _col; }
};

#endif
