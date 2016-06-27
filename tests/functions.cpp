#include <catch.hpp>
#include <luwra.hpp>

TEST_CASE("NativeFunction<R>") {
	luwra::StateWrapper state;

	REQUIRE(state.runString("return function (x, y) return x + y end") == LUA_OK);

	auto fun = state.read<luwra::NativeFunction<int>>(-1);
	REQUIRE(fun(13, 37) == 50);

	luwra::NativeFunction<double> fun2 = fun;
	REQUIRE(fun2(37.13, 13.37) == 50.5);
}

TEST_CASE("NativeFunction<void>") {
	luwra::StateWrapper state;

	REQUIRE(state.runString("return function (x, y) returnValue = x + y end") == LUA_OK);

	auto fun = state.read<luwra::NativeFunction<void>>(-1);
	fun(13, 37);

	int returnValue = state.get<int>("returnValue");
	REQUIRE(returnValue == 50);
}

TEST_CASE("function<R(A...)>") {
	luwra::StateWrapper state;

	REQUIRE(state.runString("return function (x, y) return x + y end") == LUA_OK);

	auto fun = state.read<std::function<int (int, int)>>(-1);
	REQUIRE(fun(13, 37) == 50);
}

TEST_CASE("function<void(A...)>") {
	luwra::StateWrapper state;

	REQUIRE(state.runString("return function (x, y) returnValue = x + y end") == LUA_OK);

	auto fun = state.read<std::function<void (int, int)>>(-1);
	fun(13, 37);

	int returnValue = state["returnValue"];
	REQUIRE(returnValue == 50);
}
