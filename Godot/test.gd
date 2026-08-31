extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _on_gd_example_position_changed(node: Node2D, new_pos: Vector2) -> void:
	node.rotate(deg_to_rad(1.0))
