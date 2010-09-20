#ifndef BALL_H
#define BALL_H

#define FACING_AS_FIXED 0

class Jet : public PhyObj, NetObj
{
	private:
		// drawing
		::BITMAP *_bmp;
		int _col;

		// movement
		bool _thrust;
#if FACING_AS_FIXED
		fixed  _facing; // top-word: mantissa, bot-word: exponent

		// 256 = full circle
		const fixed sensitivity;
#else
		double _facing;
#endif

	public:
		// if addr is NULL, assumed to be localhost
		Jet(double x, double y, double speed, double heading, double facing,
				::BITMAP *bmp, int col,
				struct ::sockaddr_in *addr);

		Jet(double speed, BITMAP *bmp, struct sockaddr_in *addr);
		// ^ everything else randomised

		~Jet();

		virtual void draw(::BITMAP *);
		void move(double xlim, double ylim); // override

		void thrust(bool on);
		void rotate_right(), rotate_left();
};

#endif
