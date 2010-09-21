#ifndef BALL_H
#define BALL_H

class Jet : public PhyObj, NetObj
{
	private:
		// drawing
		::BITMAP *_bmp;
		int _col;

		// movement
		bool _thrust, _firing;
		double _facing;

		long lastshot;

	public:
		// if addr is NULL, assumed to be localhost
		Jet(double x, double y, double speed, double heading, double facing,
				::BITMAP *bmp, int col,
				struct ::sockaddr_in *addr);

		Jet(double speed, BITMAP *bmp, struct sockaddr_in *addr);
		// ^ everything else randomised

		virtual ~Jet();

		virtual void draw(::BITMAP *) const;
		void move(double xlim, double ylim); // override

		void thrust(bool on);
		void rotate_right(), rotate_left();

		Bullet *createbullet();
		inline void setfiring(bool on){ _firing = on; }
		bool canfire();
};

#endif
