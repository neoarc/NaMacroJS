#pragma once

#include <Windows.h>
#include <optional>

class NaMouse
{
public:
	static POINT GetPosition();
	static void SetPosition(int x, int y);
	
	static void Move(int x, int y);
	static void Click(std::optional<int> x = std::nullopt, std::optional<int> y = std::nullopt);
	static void LbuttonDown(std::optional<int> x = std::nullopt, std::optional<int> y = std::nullopt);
	static void LbuttonUp(std::optional<int> x = std::nullopt, std::optional<int> y = std::nullopt);
	static void RbuttonDown(std::optional<int> x = std::nullopt, std::optional<int> y = std::nullopt);
	static void RbuttonUp(std::optional<int> x = std::nullopt, std::optional<int> y = std::nullopt);
	static void WheelDown(std::optional<int> x = std::nullopt, std::optional<int> y = std::nullopt);
	static void WheelUp(std::optional<int> x = std::nullopt, std::optional<int> y = std::nullopt);

	static void PostMove(int x, int y);
	static void PostClick(int x, int y);
	static void PostLbuttonDown(int x, int y);
	static void PostLbuttonUp(int x, int y);
	static void PostRbuttonDown(int x, int y);
	static void PostRbuttonUp(int x, int y);
};

