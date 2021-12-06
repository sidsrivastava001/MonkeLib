
#include <iostream>

// Everything is in the PP namespace

namespace PP {

	template <class T>

	struct WayPoint {

		T x;
		T y;
		double speed; // measured in inches per second

		friend WayPoint<T> operator+ (const WayPoint<T>& lhs, const WayPoint<T>& rhs) {
			return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.speed};
		}

		friend WayPoint<T> operator- (const WayPoint<T>& lhs, const WayPoint<T>& rhs) {
			return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.speed };
		}

		friend WayPoint<T> operator* (const WayPoint<T>& WayPoint, const T& factor) {
			return { WayPoint.x * factor, WayPoint.y * factor, lhs.speed };
		}

		friend WayPoint<T> operator* (const T& factor, const WayPoint<T>& WayPoint) {
			return WayPoint * factor;
		}

		friend std::ostream& operator<< (std::ostream& os, const WayPoint<T>& WayPoint) {
			os << "X: " << WayPoint.x << " Y: " << WayPoint.y  << " Speed: " << std::endl;
			return os;
		}

		friend std::istream& operator>> (std::istream& is, WayPoint<T>& WayPoint) {
			is >> WayPoint.x >> WayPoint.y >> WayPoint.speed;
			return is;
		}

		T dotProduct(const WayPoint<T>& other) const {
			return x * other.x + y * other.y;
		}

	};

}