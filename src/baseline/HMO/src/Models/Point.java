package Models;

public abstract class Point {
	protected int x, y;
	protected int id;
	protected int cost;

	public int getX() {
		return x;
	}

	public void setX(int x) {
		this.x = x;
	}

	public int getY() {
		return y;
	}

	public void setY(int y) {
		this.y = y;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getCost() {
		return cost;
	}

	public void setCost(int cost) {
		this.cost = cost;
	}

	public int distance(Point other) {
		return (int) (Math.sqrt(Math.pow(x - other.getX(), 2)
				+ Math.pow(y - other.getY(), 2)) * 100.0);
	}

	@Override
	public int hashCode() {
		return id + 1;
	}

	@Override
	public String toString() {
		return String.valueOf(id);
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == null) {
			return false;
		}
		Point other = (Point) obj;
		return other.hashCode() == this.hashCode();
	}
}
