#ifndef _THEIA_MATH_SQUARE_MATRIX_H_
#define _THEIA_MATH_SQUARE_MATRIX_H_
#include <stdint.h>
#include <span>

namespace Theia {
	template <typename T, int N> class SquareMatrix {
	public:
		SquareMatrix() {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					if (i == j) {
						m_data[i][j] = T(1);
					}
					else {
						m_data[i][j] = T{};
					}
				}
			}
		}
		
		template <typename T, int N> void Init(T matrix[N][N], uint32_t i, uint32_t j) {}

		template <typename T, int N, typename... Args> inline void Init(T matrix[N][N], uint32_t i, uint32_t j, T value, Args... args) {
			matrix[i][j] = value;
			
			if (++j == N) {
				++i;
				j = 0;
			}

			Init<T, N>(matrix, i, j, args...);
		}

		template <typename T, int N> inline static void InitDiagonal(T matrix[N][N], uint32_t i) {}

		template <typename T, int N, typename... Args> inline static void InitDiagonal(T matrix[N][N], uint32_t i, T value, Args... args) {
			matrix[i][i] = value;

			InitDiagonal<T, N>(matrix, i + 1, args...);
		}

		template <typename... Args> SquareMatrix(T value, Args... args) {
			static_assert(1 + sizeof...(Args) == N*N, "Incorrect number of values provided to SquareMatrix::Constructor");
			Init<T, N>(m_data, 0, 0, value, args...);
		}

		template <typename... Args> static SquareMatrix Diagonal(T value, Args... args) {
			static_assert(1 + sizeof...(Args) == N, "Incorrect number of values provided to SquareMatrix::Constructor");
			SquareMatrix matrix;
			InitDiagonal<T, N>(matrix.m_data, 0, value, args...);
			return matrix;
		}

		SquareMatrix(T diagonal_value) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] = (i == j) ? diagonal_value : T{};
				}
			}
		}

		SquareMatrix(const SquareMatrix& matrix) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] = matrix.m_data[i][j];
				}
			}
		}

		template <typename U> explicit SquareMatrix(const SquareMatrix<U, N>& matrix) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] = T(matrix.m_data[i][j]);
				}
			}
		}

		SquareMatrix& operator=(const SquareMatrix& matrix) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] = matrix.m_data[i][j];
				}
			}
			
			return *this;
		}

		bool operator==(const SquareMatrix& matrix) const {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					if (m_data[i][j] != matrix.m_data[i][j]) {
						return false;
					}
				}
			}

			return true;
		}

		bool operator!=(const SquareMatrix& matrix) const {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					if (m_data[i][j] != matrix.m_data[i][j]) {
						return true;
					}
				}
			}

			return false;
		}

		const T* operator[](uint32_t i) const {
			return m_data[i];
		}

		T* operator[](uint32_t i) {
			return m_data[i];
		}

		SquareMatrix operator+(const SquareMatrix& matrix) const {
			SquareMatrix return_matrix;
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					return_matrix.m_data[i][j] = m_data[i][j] + matrix.m_data[i][j];
				}
			}
			return return_matrix;
		}

		SquareMatrix operator-(const SquareMatrix& matrix) const {
			SquareMatrix return_matrix;
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					return_matrix.m_data[i][j] = m_data[i][j] - matrix.m_data[i][j];
				}
			}
			return return_matrix;
		}

		SquareMatrix operator*(const SquareMatrix& matrix) const {
			SquareMatrix return_matrix;
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					return_matrix.m_data[i][j] = T{};
					for (uint32_t k = 0; k < N; ++k) {
						return_matrix.m_data[i][j] += m_data[i][k] * matrix.m_data[k][j];
					}
				}
			}

			return return_matrix;
		}

		SquareMatrix operator+(T scalar) const {
			SquareMatrix return_matrix;

			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					return_matrix.m_data[i][j] = m_data[i][j] + scalar;
				}
			}

			return return_matrix;
		}

		SquareMatrix operator-(T scalar) const {
			SquareMatrix return_matrix;

			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					return_matrix.m_data[i][j] = m_data[i][j] - scalar;
				}
			}

			return return_matrix;
		}

		SquareMatrix operator*(T scalar) const {
			SquareMatrix return_matrix;

			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					return_matrix.m_data[i][j] = m_data[i][j] * scalar;
				}
			}

			return return_matrix;
		}

		SquareMatrix operator/(T scalar) const {
			SquareMatrix return_matrix;

			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					return_matrix.m_data[i][j] = m_data[i][j] / scalar;
				}
			}

			return return_matrix;
		}

		SquareMatrix& operator+=(const SquareMatrix& matrix) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] += matrix.m_data[i][j];
				}
			}
			
			return *this;
		}

		SquareMatrix& operator-=(const SquareMatrix& matrix) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] -= matrix.m_data[i][j];
				}
			}

			return *this;
		}

		SquareMatrix& operator*=(const SquareMatrix& matrix) {
			*this = *this * matrix;
			return *this;
		}

		SquareMatrix& operator+=(T scalar) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] += scalar;
				}
			}
			return *this;
		}

		SquareMatrix& operator-=(T scalar) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] -= scalar;
				}
			}
			return *this;
		}

		SquareMatrix& operator*=(T scalar) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] *= scalar;
				}
			}
			return *this;
		}

		SquareMatrix& operator/=(T scalar) {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					m_data[i][j] /= scalar;
				}
			}
			return *this;
		}

		bool IsIdentity() const {
			for (uint32_t i = 0; i < N; ++i) {
				for (uint32_t j = 0; j < N; ++j) {
					if (i == j && m_data[i][j] != 1.0) {
						return false;
					}
					else if (i != j && m_data[i][j] != 0.0) {
						return false;
					}
				}
			}
			return true;
		}

	private:
		T m_data[N][N];
	};

	template <typename T, int N> SquareMatrix<T, N> operator+(T scalar, const Theia::SquareMatrix<T, N>& matrix) {
		Theia::SquareMatrix<T, N> return_matrix;

		for (uint32_t i = 0; i < N; ++i) {
			for (uint32_t j = 0; j < N; ++j) {
				return_matrix[i][j] = scalar + matrix[i][j];
			}
		}

		return return_matrix;
	}

	template <typename T, int N> SquareMatrix<T, N> operator-(T scalar, const Theia::SquareMatrix<T, N>& matrix) {
		Theia::SquareMatrix<T, N> return_matrix;

		for (uint32_t i = 0; i < N; ++i) {
			for (uint32_t j = 0; j < N; ++j) {
				return_matrix[i][j] = scalar - matrix[i][j];
			}
		}

		return return_matrix;
	}

	template <typename T, int N> SquareMatrix<T, N> operator*(T scalar, const Theia::SquareMatrix<T, N>& matrix) {
		Theia::SquareMatrix<T, N> return_matrix;

		for (uint32_t i = 0; i < N; ++i) {
			for (uint32_t j = 0; j < N; ++j) {
				return_matrix[i][j] = scalar * matrix[i][j];
			}
		}

		return return_matrix;
	}

	template <typename T, int N> SquareMatrix<T, N> operator/(T scalar, const Theia::SquareMatrix<T, N>& matrix) {
		Theia::SquareMatrix<T, N> return_matrix;

		for (uint32_t i = 0; i < N; ++i) {
			for (uint32_t j = 0; j < N; ++j) {
				return_matrix[i][j] = matrix[i][j] / scalar;
			}
		}

		return return_matrix;
	}

	template <typename T, int N> SquareMatrix<T, N> Transpose(const SquareMatrix<T, N>& matrix) {
		SquareMatrix<T, N> return_matrix;

		for (uint32_t i = 0; i < N; ++i) {
			for (uint32_t j = 0; j < N; ++j) {
				return_matrix[i][j] = matrix[j][i];
			}
		}

		return return_matrix;
	}

	template <typename T, int N> SquareMatrix<T, N> Determinant(const SquareMatrix<T, N>& matrix) {
		//TODO Implement Matrix Determinant Computation.
		return SquareMatrix<T, N>();
	}

	template <typename T, int N> SquareMatrix<T, N> Inverse(const SquareMatrix<T, N>& matrix) {
		//TODO Implement Inverse Matrix Computation.

		return SquareMatrix<T, N>();
	}
}
#endif