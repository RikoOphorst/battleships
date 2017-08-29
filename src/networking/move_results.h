#pragma once

enum EMoveResult
{
	EMoveResult_Success = 0,
	EMoveResult_HitEdge,
	EMoveResult_HitShip,
	EMoveResult_HitMine,
	EMoveResult_Failed
};
