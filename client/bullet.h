#ifndef BULLET_H
#define BULLET_H

class Bullet : public PhyObj
{
	private:
		int _col;
		long _maketime;

	public:
		Bullet(double x, double y,
				double speed, double heading, int col);
		virtual ~Bullet();

		virtual void draw(BITMAP *) const;
		virtual bool expired()      const;
};

#endif
