extends Node2D

var movegen: GDMoveGenerator
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	movegen = get_child(0)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	print(movegen.generate_moves(1))
