using Godot;
using Godot.Collections;
using Array = System.Array;

public partial class MoveGeneration : Node
{
	public Array<int> GenerateMoves()
	{
		Array<int> moves = [];
		moves.Add(32);
		return moves;
	}
}
