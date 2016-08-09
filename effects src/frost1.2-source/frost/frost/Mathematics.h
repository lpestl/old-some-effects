
#ifndef __MATHEMATICS_H__
#define __MATHEMATICS_H__

#include <iostream>
#include "FloatTraits.h"



// Подобное решение вопроса с представлением матриц основано на том факте,
// что размерности векторов и матриц поставленной задачи всегда известны заранее.
// Таким образом, задавая явно размеры входящих в задачу векторов и матриц,
// мы ни когда не получим на выходе матрицы "случайных" размерностей, а наоборот
// мы всегда знаем все размеры матриц как промежуточного, так и конечного результата.



#define FOREACH(SIZE) for( int i=0; i<SIZE; ++i )
#define FOREACH_IDX(IDX,SIZE) for( int IDX=0; IDX<SIZE; ++IDX )

#define FOREACH2(R,C)  for( int i=0; i<R; ++i ) for( int j=0; j<C; ++j )
#define FOREACH_IDX2(IDX,JDX,R,C)  for( int IDX=0; IDX<R; ++IDX ) for( int JDX=0; JDX<C; ++JDX )

#define GET(MATRIX,I,J) MATRIX.m_v[I*MATRIX.m_cols + J]


template<typename TYPE>
inline TYPE _clamp(TYPE value, TYPE min, TYPE max) {
	return value < min ? min : (value > max ? max : value);
}

inline float _clampf(float value, float min, float max) {
	return _clamp<float>( value, min, max );
}

inline double _clampd(double value, double min, double max) {
	return _clamp<double>( value, min, max );
}

inline int _clampi(int value, int min, int max) {
	return _clamp<int>( value, min, max );
}

inline float _round(float value) {
	return value > 0 ? floor(value + 0.5f) : floor(value - 0.5f);
}



template<typename FLOAT, short ROWS, short COLS>
class ArrayOp
{
public:
	enum {
		m_rows = ROWS,
		m_cols = COLS,
		m_size = ROWS * COLS,
	};

	static void Assign(FLOAT left[m_size], FLOAT right) {
		FOREACH(m_size)
			left[i] = right;
	}

	template<short R2, short C2>
	static void Assign(FLOAT left[m_size], const FLOAT right[R2 * C2])
	{
		short minRows = __min(ROWS, R2);
		short minCols = __min(COLS, C2);

		FOREACH2(minRows,minCols)
			left[i*COLS + j] = right[i*C2 + j];
	}

	static void Add(FLOAT left[m_size], const FLOAT right[m_size]) {
		FOREACH(m_size)
			left[i] += right[i];
	}

	static void Sub(FLOAT left[m_size], const FLOAT right[m_size]) {
		FOREACH(m_size)
			left[i] -= right[i];
	}

	static void Mul(FLOAT left[m_size], FLOAT right) {
		FOREACH(m_size)
			left[i] *= right;
	}

	static void Div(FLOAT left[m_size], FLOAT right) {
		if( abs(right) < FloatTraits<FLOAT>::epsilon )
			return;
		FOREACH(m_size)
			left[i] /= right;
	}
};


//
// Матрица размером ROWS x COLS
//
template<typename FLOAT, short ROWS, short COLS>
class Mat
{
public:
	typedef Mat<FLOAT,ROWS,COLS> this_t;
	typedef ArrayOp<FLOAT,ROWS,COLS> array_t;

	//
	// Класс для доступа к строке row через оператор []
	// ------------------------------------------------
	class Accessor
	{
	public:
		Accessor(this_t& mat, int row): m_mat(mat), m_row(row) {}
		FLOAT& operator [] (int col) {
			return GET(m_mat,m_row,col);
		}
	private:
		this_t& m_mat;
		int m_row;
	};

	//
	// Константная версия класса доступа
	// ---------------------------------
	class ConstAccessor
	{
	public:
		ConstAccessor(const this_t& mat, int row): m_mat(mat), m_row(row) {}
		FLOAT operator [] (int col) const {
			return GET(m_mat,m_row,col);
		}
	private:
		const this_t& m_mat;
		int m_row;
	};

private:
	void operator = (FLOAT v[]);

public:
	FLOAT m_v[ROWS*COLS];
	enum {
		m_rows = ROWS,
		m_cols = COLS,
		m_size = ROWS * COLS,
	};
	
	//
	// Конструкторы
	// ------------
	Mat(FLOAT f = (FLOAT)0) {
		array_t::Assign(m_v, f);
	}
	Mat(FLOAT v[m_size]) {
		array_t::Assign<ROWS,COLS>(m_v, v);
	}
	template<short R2, short C2>
	Mat(const Mat<FLOAT,R2,C2>& other)
	{
		memset(m_v, 0, sizeof(FLOAT) * m_size);
		array_t::Assign<R2,C2>(m_v, other.m_v);
	}

	//
	// Операторы присваивания
	// ----------------------
	this_t& operator = (FLOAT f) {
		array_t::Assign(m_v, f);
		return *this;
	}
	template<short R2, short C2>
	this_t& operator = (const Mat<FLOAT,R2,C2>& other)
	{
		array_t::Assign<R2,C2>(m_v, other.m_v);
		return *this;
	}
	this_t& operator += (const this_t& other) {
		array_t::Add(m_v, other.m_v);
		return *this;
	}
	this_t& operator -= (const this_t& other) {
		array_t::Sub(m_v, other.m_v);
		return *this;
	}
	this_t& operator *= (FLOAT value) {
		array_t::Mul(m_v, value);
		return *this;
	}
	this_t& operator /= (FLOAT value) {
		array_t::Div(m_v, value);
		return *this;
	}

	//
	// Операторы обращения по индексу
	// ------------------------------
	FLOAT const operator () (int i, int j) const {
		return m_v[i*COLS + j];
	}
	FLOAT& operator () (int i, int j) {
		return m_v[i*COLS + j];
	}
	ConstAccessor operator [] (int i) const {
		return ConstAccessor(*this, i);
	}
	Accessor operator [] (int i) {
		return Accessor(*this, i);
	}
};

template<typename FLOAT, short ROWS, short COLS> std::ostream& operator << (std::ostream& ost, Mat<FLOAT,ROWS,COLS> mat)
{
	for( int i=0; i<ROWS; ++i )	{
		for( int j=0; j<COLS; ++j )
			ost << GET(mat,i,j) << ", ";
		ost << std::endl;
	}
	return ost;
}


//
// Вектор-столбец размера SIZE
//
template<typename FLOAT, short SIZE>
class Mat<FLOAT,SIZE,1>
{
public:
	typedef Mat<FLOAT,SIZE,1> this_t;
	typedef ArrayOp<FLOAT,SIZE,1> array_t;

	FLOAT m_v[SIZE];
	enum {
		m_rows = SIZE,
		m_cols = 1,
		m_size = SIZE,
	};

	//
	// Конструкторы
	// ------------
	Mat(FLOAT f = (FLOAT)0) {
		array_t::Assign(m_v, f);
	}
	Mat(FLOAT v[SIZE]) {
		array_t::Assign<m_size,1>(m_v, v);
	}
	template<short SIZE2>
	Mat(const Mat<FLOAT,SIZE2,1>& other)
	{
		memset(m_v, 0, sizeof(FLOAT) * SIZE);
		array_t::Assign<SIZE2,1>(m_v, other.m_v);
	}

	//
	// Операторы присваивания
	// ----------------------
	this_t& operator = (FLOAT f) {
		array_t::Assign(m_v, f);
		return *this;
	}
	template<short SIZE2>
	this_t& operator = (const Mat<FLOAT,1,SIZE2>& other)
	{
		array_t::Assign<SIZE2,1>(m_v, other.m_v);
		return *this;
	}
	this_t& operator += (const this_t& other) {
		array_t::Add(m_v, other.m_v);
		return *this;
	}
	this_t& operator -= (const this_t& other) {
		array_t::Sub(m_v, other.m_v);
		return *this;
	}
	this_t& operator *= (FLOAT value) {
		array_t::Mul(m_v, value);
		return *this;
	}
	this_t& operator /= (FLOAT value) {
		array_t::Div(m_v, value);
		return *this;
	}

	//
	// Операторы обращения по индексу
	// ------------------------------
	FLOAT operator () (int idx) const {
		return m_v[idx];
	}
	FLOAT& operator () (int idx) {
		return m_v[idx];
	}
	FLOAT operator [] (int idx) const {
		return m_v[idx];
	}
	FLOAT& operator [] (int idx) {
		return m_v[idx];
	}
};

//
// Вектор-строка размера SIZE
//
template<typename FLOAT, short SIZE>
class Mat<FLOAT,1,SIZE>
{
private:
	void operator = (const FLOAT v[]);

public:
	typedef Mat<FLOAT,1,SIZE> this_t;
	typedef ArrayOp<FLOAT,1,SIZE> array_t;

	FLOAT m_v[SIZE];
	enum {
		m_rows = 1,
		m_cols = SIZE,
		m_size = SIZE,
	};

	//
	// Конструкторы
	// ------------
	Mat(FLOAT f = (FLOAT)0) {
		array_t::Assign(m_v, f);
	}
	Mat(FLOAT v[SIZE]) {
		array_t::Assign<1,SIZE>(m_v, v);
	}
	template<short SIZE2>
	Mat(const Mat<FLOAT,1,SIZE2>& other)
	{
		memset(m_v, 0, sizeof(FLOAT) * SIZE);
		array_t::Assign<1,SIZE2>(m_v, other.m_v);
	}

	//
	// Операторы присваивания
	// ----------------------
	this_t& operator = (FLOAT f) {
		array_t::Assign(m_v, f);
		return *this;
	}
	template<short SIZE2>
	this_t& operator = (const Mat<FLOAT,1,SIZE2>& other)
	{
		array_t::Assign<1,SIZE2>(m_v, other.m_v);
		return *this;
	}
	this_t& operator += (const this_t& other) {
		array_t::Add(m_v, other.m_v);
		return *this;
	}
	this_t& operator -= (const this_t& other) {
		array_t::Sub(m_v, other.m_v);
		return *this;
	}
	this_t& operator *= (FLOAT value) {
		array_t::Mul(m_v, value);
		return *this;
	}
	this_t& operator /= (FLOAT value) {
		array_t::Div(m_v, value);
		return *this;
	}

	//
	// Операторы доступа по индексу
	// ----------------------------
	FLOAT operator () (int idx) const {
		return m_v[idx];
	}
	FLOAT& operator () (int idx) {
		return m_v[idx];
	}
	FLOAT operator [] (int idx) const {
		return m_v[idx];
	}
	FLOAT& operator [] (int idx) {
		return m_v[idx];
	}
};

//
// Матрица-число
//
template<typename FLOAT>
class Mat<FLOAT,1,1>
{
public:
	typedef Mat<FLOAT,1,1> this_t;

	FLOAT m_v[1];
	enum {
		m_rows = 1,
		m_cols = 1,
		m_size = 1,
	};

	Mat(FLOAT f = (FLOAT)0) {
		m_v[0] = f;
	}

	this_t& operator += (const this_t& other) {
		m_v[0] += other.m_v[0];
		return *this;
	}
	this_t& operator -= (const this_t& other) {
		m_v[0] -= other.m_v[0];
		return *this;
	}
	this_t& operator *= (FLOAT value) {
		m_v[0] *= value;
		return *this;
	}
	this_t& operator /= (FLOAT value) {
		if( abs(value) < FloatTraits<FLOAT>::epsilon )
			return *this;
		m_v[0] /= value;
		return *this;
	}

	operator FLOAT () const {
		return m_v[0];
	}
	operator FLOAT& () {
		return m_v[0];
	}
};


template<typename FLOAT, short ROWS, short COLS>
Mat<FLOAT,ROWS,COLS> operator + (const Mat<FLOAT,ROWS,COLS>& A, const Mat<FLOAT,ROWS,COLS>& B)
{
	static Mat<FLOAT,ROWS,COLS> C;
	C = A;
	return C += B;
}

template<typename FLOAT, short ROWS, short COLS>
Mat<FLOAT,ROWS,COLS> operator - (const Mat<FLOAT,ROWS,COLS>& A, const Mat<FLOAT,ROWS,COLS>& B)
{
	static Mat<FLOAT,ROWS,COLS> C;
	C = A;
	return C -= B;
}

template<typename FLOAT, short ROWS, short COLS>
Mat<FLOAT,ROWS,COLS> operator * (const Mat<FLOAT,ROWS,COLS>& A, FLOAT f)
{
	static Mat<FLOAT,ROWS,COLS> C;
	C = A;
	return C *= f;
}

template<typename FLOAT, short ROWS, short COLS>
Mat<FLOAT,ROWS,COLS> operator * (FLOAT f, const Mat<FLOAT,ROWS,COLS>& A)
{
	static Mat<FLOAT,ROWS,COLS> C;
	C = A;
	return C *= f;
}

template<typename FLOAT, short ROWS, short COLS>
Mat<FLOAT,ROWS,COLS> operator / (const Mat<FLOAT,ROWS,COLS>& A, FLOAT f)
{
	static Mat<FLOAT,ROWS,COLS> C;
	C = A;
	return C /= f;
}

template<typename FLOAT, short ROWS, short SIZE, short COLS>
Mat<FLOAT,ROWS,COLS> operator * (const Mat<FLOAT,ROWS,SIZE>& A, const Mat<FLOAT,SIZE,COLS>& B)
{
	static Mat<FLOAT,ROWS,COLS> C;
	FOREACH2(ROWS, COLS)
	{
		GET(C,i,j) = 0;
		FOREACH_IDX(k,SIZE)
			GET(C,i,j) += GET(A,i,k) * GET(B,k,j);
	}
	return C;
}

template<typename FLOAT, short SIZE>
FLOAT DotProduct(const Mat<FLOAT,SIZE,1>& A, const Mat<FLOAT,SIZE,1>& B)
{
	FLOAT dot = 0;
	FOREACH(SIZE)
		dot += A.m_v[i] * B.m_v[i];
	return dot;
}

template<typename FLOAT, short ROWS>
FLOAT LengthSquared(const Mat<FLOAT,ROWS,1>& vec)
{
	return DotProduct(vec,vec);
}

template<typename FLOAT, short ROWS>
FLOAT Length(const Mat<FLOAT,ROWS,1>& vec)
{
	return sqrt( LengthSquared(vec) );
}

template<typename FLOAT, short ROWS>
Mat<FLOAT,ROWS,1> Normalize(const Mat<FLOAT,ROWS,1>& vec)
{
	static Mat<FLOAT,ROWS,1> ort;
	FLOAT length = Length( vec );

	if( length < FloatTraits<FLOAT>::epsilon )
		return vec;
	FOREACH(ROWS)
		ort.m_v[i] = vec.m_v[i] / length;
	return ort;
}

template<typename FLOAT>
Mat<FLOAT,3,1> CrossProduct(const Mat<FLOAT,3,1>& A, const Mat<FLOAT,3,1>& B)
{
	static Mat<FLOAT,3,1> cross;
	cross.m_v[0] = A.m_v[1] * B.m_v[2] - A.m_v[2] * B.m_v[1];
	cross.m_v[1] = A.m_v[2] * B.m_v[0] - A.m_v[0] * B.m_v[2];
	cross.m_v[2] = A.m_v[0] * B.m_v[1] - A.m_v[1] * B.m_v[0];
	return cross;
}



template<typename FLOAT, short SIZE>
FLOAT DotProduct(const Mat<FLOAT,1,SIZE>& A, const Mat<FLOAT,1,SIZE>& B)
{
	FLOAT dot = 0;
	FOREACH(SIZE)
		dot += A.m_v[i] * B.m_v[i];
	return dot;
}

template<typename FLOAT, short COLS>
FLOAT LengthSquared(const Mat<FLOAT,1,COLS>& row)
{
	return DotProduct(row,row);
}

template<typename FLOAT, short COLS>
FLOAT Length(const Mat<FLOAT,1,COLS>& row)
{
	return sqrt( LengthSquared(row) );
}

template<typename FLOAT, short COLS>
Mat<FLOAT,1,COLS> Normalize(const Mat<FLOAT,1,COLS>& row)
{
	static Mat<FLOAT,1,COLS> ort;
	FLOAT length = Length( row );

	if( length < FloatTraits<FLOAT>::epsilon )
		return row;
	FOREACH(ROWS)
		ort.m_v[i] = row.m_v[i] / length;
	return ort;
}

template<typename FLOAT>
Mat<FLOAT,1,3> CrossProduct(const Mat<FLOAT,1,3>& A, const Mat<FLOAT,1,3>& B)
{
	static Mat<FLOAT,1,3> cross;
	cross.m_v[0] = A.m_v[1] * B.m_v[2] - A.m_v[2] * B.m_v[1];
	cross.m_v[1] = A.m_v[2] * B.m_v[0] - A.m_v[0] * B.m_v[2];
	cross.m_v[2] = A.m_v[0] * B.m_v[1] - A.m_v[1] * B.m_v[0];
	return cross;
}



template<typename FLOAT, short ROWS, short COLS>
Mat<FLOAT,COLS,ROWS> Transpose(const Mat<FLOAT,ROWS,COLS>& A)
{
	static Mat<FLOAT,COLS,ROWS> AT;
	FOREACH2(ROWS,COLS)
		GET(AT,j,i) = GET(A,i,j);
	return AT;
}


template<typename FLOAT>
Mat<FLOAT,2,1> MakeVec(FLOAT x1, FLOAT x2)
{
	static Mat<FLOAT,2,1> vec;
	vec.m_v[0] = x1;
	vec.m_v[1] = x2;
	return vec;
}

template<typename FLOAT>
Mat<FLOAT,3,1> MakeVec(FLOAT x1, FLOAT x2, FLOAT x3)
{
	static Mat<FLOAT,3,1> vec;
	vec.m_v[0] = x1;
	vec.m_v[1] = x2;
	vec.m_v[2] = x3;
	return vec;
}

template<typename FLOAT>
Mat<FLOAT,1,2> MakeRow(FLOAT x1, FLOAT x2)
{
	static Mat<FLOAT,1,2> row;
	row.m_v[0] = x1;
	row.m_v[1] = x2;
	return row;
}

template<typename FLOAT>
Mat<FLOAT,1,3> MakeRow(FLOAT x1, FLOAT x2, FLOAT x3)
{
	static Mat<FLOAT,1,3> row;
	row.m_v[0] = x1;
	row.m_v[1] = x2;
	row.m_v[2] = x3;
	return row;
}

template<typename FLOAT>
Mat<FLOAT,2,2> MakeMat(FLOAT xx, FLOAT xy, FLOAT yx, FLOAT yy)
{
	static Mat<FLOAT,2,2> mat;
	GET(mat,0,0) = xx;
	GET(mat,0,1) = xy;
	GET(mat,1,0) = yx;
	GET(mat,1,1) = yy;
	return mat;
}

template<typename FLOAT>
Mat<FLOAT,3,3> MakeMat(FLOAT xx, FLOAT xy, FLOAT xz, FLOAT yx, FLOAT yy, FLOAT yz, FLOAT zx, FLOAT zy, FLOAT zz)
{
	static Mat<FLOAT,3,3> mat;
	GET(mat,0,0) = xx;
	GET(mat,0,1) = xy;
	GET(mat,0,2) = xz;

	GET(mat,1,0) = yx;
	GET(mat,1,1) = yy;
	GET(mat,1,2) = yz;

	GET(mat,2,0) = zx;
	GET(mat,2,1) = zy;
	GET(mat,2,2) = zz;
	return mat;
}

template<typename FLOAT>
Mat<FLOAT,2,2> MakeMat(const Mat<FLOAT,2,1>& v1, const Mat<FLOAT,2,1>& v2)
{
	return MakeMat(v1[0], v2[0], v1[1], v2[1]);
}

template<typename FLOAT>
Mat<FLOAT,2,2> MakeMat(const Mat<FLOAT,1,2>& r1, const Mat<FLOAT,1,2>& r2)
{
	return MakeMat(r1[0], r1[1], r2[0], r2[1]);
}

template<typename FLOAT>
Mat<FLOAT,3,3> MakeMat(const Mat<FLOAT,3,1>& v1, const Mat<FLOAT,3,1>& v2, const Mat<FLOAT,3,1>& v3)
{
	return MakeMat(v1[0], v2[0], v3[0], v1[1], v2[1], v3[1], v1[2], v2[2], v3[2]);
}

template<typename FLOAT>
Mat<FLOAT,3,3> MakeMat(const Mat<FLOAT,1,3>& r1, const Mat<FLOAT,1,3>& r2, const Mat<FLOAT,1,3>& r3)
{
	return MakeMat(r1[0], r1[1], r1[2], r2[0], r2[1], r2[2], r3[0], r3[1], r3[2]);
}

typedef Mat<float,2,2> Mat22f;
typedef Mat<float,3,3> Mat33f;
typedef Mat<float,2,3> Mat23f;
typedef Mat<float,3,2> Mat32f;

typedef Mat<float,2,1> Vec2f;
typedef Mat<float,1,2> Row2f;

typedef Mat<float,3,1> Vec3f;
typedef Mat<float,1,3> Row3f;

typedef Mat<float,4,1> Vec4f;
typedef Mat<float,1,4> Row4f;


#undef FOREACH
#undef FOREACH_IDX
#undef FOREACH2
#undef FOREACH_IDX2
#undef GET

#endif //__MATHEMATICS_H__