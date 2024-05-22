#include "Math.h"

namespace Theia {
	Mat4 CreateMat4(Theia::Float m00, Theia::Float m01, Theia::Float m02, Theia::Float m03,
		Theia::Float m10, Theia::Float m11, Theia::Float m12, Theia::Float m13,
		Theia::Float m20, Theia::Float m21, Theia::Float m22, Theia::Float m23,
		Theia::Float m30, Theia::Float m31, Theia::Float m32, Theia::Float m33
	) {
		Mat4 mat;
		mat[0][0] = m00;
		mat[0][1] = m01;
		mat[0][2] = m02;
		mat[0][3] = m03;

		mat[1][0] = m10;
		mat[1][1] = m11;
		mat[1][2] = m12;
		mat[1][3] = m13;

		mat[2][0] = m20;
		mat[2][1] = m21;
		mat[2][2] = m22;
		mat[2][3] = m23;

		mat[3][0] = m30;
		mat[3][1] = m31;
		mat[3][2] = m32;
		mat[3][3] = m33;
		return mat;
	}

	std::string Mat4ToSting(Theia::Mat4& matrix) {
		std::string return_string;

		for (uint32_t i = 0; i < 4; ++i) {
			for (uint32_t j = 0; j < 4; ++j) {
				return_string += std::to_string(matrix[i][j]) + " ";
			}
			return_string += "\n";
		}

		return return_string;
	}

	Theia::Transform Translate(const Theia::Vector3f& delta) {
		Theia::Mat4 matrix = CreateMat4(1.0f, 0.0f, 0.0f, delta.m_x,
										0.0f, 1.0f, 0.0f, delta.m_y,
										0.0f, 0.0f, 1.0f, delta.m_z,
										0.0f, 0.0f, 0.0f, 1.0f
		);

		Theia::Mat4 inverse_matrix = CreateMat4(1.0f, 0.0f, 0.0f, -delta.m_x,
												0.0f, 1.0f, 0.0f, -delta.m_y,
												0.0f, 0.0f, 1.0f, -delta.m_z,
												0.0f, 0.0f, 0.0f, 1.0f
		);

		return Theia::Transform(matrix, inverse_matrix);
	}

	Theia::Transform Scale(const Theia::Vector3f& delta) {
		Theia::Mat4 matrix = CreateMat4(delta.m_x, 0.0f, 0.0f, 0.0f,
										0.0f, delta.m_y, 0.0f, 0.0f,
										0.0f, 0.0f, delta.m_z, 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f
		);

		Theia::Mat4 inverse_matrix = CreateMat4(1.0f / delta.m_x, 0.0f, 0.0f, 0.0f,
												0.0f, 1.0f / delta.m_y, 0.0f, 0.0f,
												0.0f, 0.0f, 1.0f / delta.m_z, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f
		);

		return Theia::Transform(matrix, inverse_matrix);
	}

	Theia::Transform RotateXAxis(Theia::Float theta) {
		Theia::Float sin_theta = std::sin(theta);
		Theia::Float cos_theta = std::cos(theta);

		Mat4 matrix = CreateMat4(1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, cos_theta, -sin_theta, 0.0f,
								 0.0f, sin_theta, cos_theta, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f
		);

		Mat4 inverse_matrix = Transpose(matrix);

		return Transform(matrix, inverse_matrix);
	}

	Theia::Transform RotateYAxis(Theia::Float theta) {
		Theia::Float sin_theta = std::sin(theta);
		Theia::Float cos_theta = std::cos(theta);

		Mat4 matrix = CreateMat4(cos_theta, 0.0f, sin_theta, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 -sin_theta, 0.0f, cos_theta, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f
		);

		Mat4 inverse_matrix = Transpose(matrix);

		return Transform(matrix, inverse_matrix);
	}

	Theia::Transform RotateZAxis(Theia::Float theta) {
		Theia::Float sin_theta = std::sin(theta);
		Theia::Float cos_theta = std::cos(theta);

		Mat4 matrix = CreateMat4(cos_theta, -sin_theta, 0.0f, 0.0f,
								 sin_theta, cos_theta, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f
		);

		Mat4 inverse_matrix = Transpose(matrix);

		return Transform(matrix, inverse_matrix);
	}

	Theia::Transform Rotate(Theia::Float sin_theta, Theia::Float cos_theta, const Vector3f& axis) {
		Vector3f a = Normalize(axis);
		Mat4 matrix = Mat4(1.0f);

		matrix[0][0] = a.m_x * a.m_x + (1.0f - a.m_x * a.m_x) * cos_theta;
		matrix[0][1] = a.m_x * a.m_y * (1.0f - cos_theta) - a.m_z * sin_theta;
		matrix[0][2] = a.m_x * a.m_z * (1.0f - cos_theta) - a.m_y * sin_theta;
		matrix[0][3] = 0.0f;

		matrix[1][0] = a.m_x * a.m_y * (1 - cos_theta) + a.m_z * sin_theta;
		matrix[1][1] = a.m_y * a.m_y + (1 - a.m_y * a.m_y) * cos_theta;
		matrix[1][2] = a.m_y * a.m_z * (1 - cos_theta) - a.m_x * sin_theta;
		matrix[1][3] = 0;

		matrix[2][0] = a.m_x * a.m_z * (1 - cos_theta) - a.m_y * sin_theta;
		matrix[2][1] = a.m_y * a.m_z * (1 - cos_theta) + a.m_x * sin_theta;
		matrix[2][2] = a.m_z * a.m_z + (1 - a.m_z * a.m_z) * cos_theta;
		matrix[2][3] = 0;

		return Transform(matrix, Transpose(matrix));
	}

	Theia::Transform Rotate(Theia::Float theta, const Vector3f& axis) {
		Theia::Float sin_theta = std::sin(theta);
		Theia::Float cos_theta = std::cos(theta);
		return Rotate(sin_theta, cos_theta, axis);
	}

	Theia::Transform LookAt(const Point3f& position, const Point3f& target, const Vector3f& up) {
		Mat4 inverse_matrix = Mat4(1.0f);

		inverse_matrix[0][3] = position.m_x;
		inverse_matrix[1][3] = position.m_y;
		inverse_matrix[2][3] = position.m_z;

		Vector3f camera_direction = Normalize(target - position);
		Vector3f camera_right = Normalize(Cross(Normalize(up), camera_direction));
		Vector3f camera_up = Cross(camera_direction, camera_right);

		inverse_matrix[0][0] = camera_right.m_x;
		inverse_matrix[1][0] = camera_right.m_y;
		inverse_matrix[2][0] = camera_right.m_z;

		inverse_matrix[0][1] = camera_up.m_x;
		inverse_matrix[1][1] = camera_up.m_y;
		inverse_matrix[2][1] = camera_up.m_z;

		inverse_matrix[0][2] = camera_direction.m_x;
		inverse_matrix[1][2] = camera_direction.m_y;
		inverse_matrix[2][2] = camera_direction.m_z;

		return Transform(Inverse(inverse_matrix), inverse_matrix);
	}

	Theia::Transform LookAt(const Point3f& position, const Vector3f& direction, const Vector3f& up) {
		Mat4 inverse_matrix = Mat4(1.0f);

		inverse_matrix[0][3] = position.m_x;
		inverse_matrix[1][3] = position.m_y;
		inverse_matrix[2][3] = position.m_z;

		Vector3f camera_direction = Normalize(direction);
		Vector3f camera_right = Normalize(Cross(Normalize(up), camera_direction));
		Vector3f camera_up = Cross(camera_direction, camera_right);

		inverse_matrix[0][0] = camera_right.m_x;
		inverse_matrix[1][0] = camera_right.m_y;
		inverse_matrix[2][0] = camera_right.m_z;

		inverse_matrix[0][1] = camera_up.m_x;
		inverse_matrix[1][1] = camera_up.m_y;
		inverse_matrix[2][1] = camera_up.m_z;

		inverse_matrix[0][2] = camera_direction.m_x;
		inverse_matrix[1][2] = camera_direction.m_y;
		inverse_matrix[2][2] = camera_direction.m_z;

		return Transform(Inverse(inverse_matrix), inverse_matrix);
	}
}