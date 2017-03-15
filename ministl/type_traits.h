#ifndef TYPE_TRAITS
#define TYPE_TRAITS

namespace ministl
{
	struct false_type
	{
		// empty
	};

	struct true_type
	{
		// empty
	};

	template <typename T>
	struct is_integral: false_type
	{

	};

	template <>
	struct is_integral<bool>: true_type
	{

	};

	template <>
	struct is_integral<char>: true_type
	{

	};


	template <>
	struct is_integral<int>: true_type
	{

	};


	template <>
	struct is_integral<short>: true_type
	{

	};


	template <>
	struct is_integral<long>: true_type
	{

	};


	template <>
	struct is_integral<long long>: true_type
	{

	};


	template <typename T>
	struct type_traits
	{
		typedef false_type is_trivally_default_constructible;
		typedef false_type is_trivally_copy_constructible;
		typedef false_type is_trivally_assigmentable;
		typedef false_type is_trivially_destructible;
		typedef false_type is_POD_type;
	};

	template <>
	struct type_traits<char>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<signed char>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<unsigned char>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<short>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<unsigned short>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<int>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<unsigned int>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<long>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<unsigned long>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<float>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<double>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<long double>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};

	template <typename T>
	struct type_traits<T*>
	{
		typedef true_type is_trivally_default_constructible;
		typedef true_type is_trivally_copy_constructible;
		typedef true_type is_trivally_assigmentable;
		typedef true_type is_trivially_destructible;
		typedef true_type is_POD_type;
	};
}

#endif