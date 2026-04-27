import unreal
 
def create_fade_material(original_material, fade_param_name, fade_material_suffix="_Fade", fade_material_instance_prefix="MI_"):
    # Get the original material's package path
    material_path = original_material.get_path_name()
    package_path = unreal.Paths.get_path(material_path)
    
    # Generate a new name and path for the duplicated material
    original_name = original_material.get_name()
    new_name = f"{original_name}{fade_material_suffix}"
    new_material_path = f"{package_path}/{new_name}"
    
    # Duplicate the original material
    new_material = unreal.EditorAssetLibrary.duplicate_asset(material_path, new_material_path)
    
    if not new_material:
        unreal.log_error(f"Failed to duplicate material: {original_name}")
        return
    
    # Set Blend Mode to Masked
    new_material.set_editor_property('blend_mode', unreal.BlendMode.BLEND_MASKED)
    
    # Create a Scalar Parameter for the fade value
    fade_param = unreal.MaterialEditingLibrary.create_material_expression(new_material, unreal.MaterialExpressionScalarParameter, -400, 200)
    fade_param.set_editor_property('parameter_name', fade_param_name)
    fade_param.set_editor_property('default_value', 1.0)
    
    # Ensure the parameter is properly created
    if fade_param is None:
        unreal.log_error("Failed to create the fade parameter")
        return
    
    dither_node = unreal.MaterialEditingLibrary.create_material_expression(new_material, unreal.MaterialExpressionMaterialFunctionCall, -400, 200)
     # Ensure the dither node is properly created
    if dither_node is None:
        unreal.log_error("Failed to create the DitherNode")
        return


    # Load the specific DitherTemporalAA function from the engine content
    dither_func = unreal.EditorAssetLibrary.load_asset("/Engine/Functions/Engine_MaterialFunctions02/Utility/DitherTemporalAA.DitherTemporalAA")
    # Set the node to use that function
    dither_node.set_editor_property("material_function", dither_func)

    # Get the material expression for opacity
    # Connect Scalar Parameter to Dither Node
    unreal.MaterialEditingLibrary.connect_material_expressions(fade_param, "", dither_node, "Alpha Threshold")
    # Connect Dither Node to Opacity Mask
    unreal.MaterialEditingLibrary.connect_material_property(dither_node, '', unreal.MaterialProperty.MP_OPACITY_MASK)
    
    # Save the new material
    unreal.EditorAssetLibrary.save_asset(new_material_path)
        
    # Log success
    unreal.log(f"Created fade material: {new_material_path}")
    
    create_fade_material_instance(new_material, fade_material_instance_prefix, package_path, new_name)
 
 
def create_fade_material_instance(fade_material, fade_material_instance_prefix, package_path, new_name):
    # Generate the name for the new material instance
    fade_material_name = fade_material.get_name()
    new_name_no_prefix = new_name.removeprefix("M_")
    instance_name = f"{fade_material_instance_prefix}{new_name_no_prefix}"
    
    new_instance_path = f"{package_path}/{instance_name}"
    
    # Create a material instance constant
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    material_instance = asset_tools.create_asset(
        instance_name, package_path, unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew()
    )
    
    if not material_instance:
        unreal.log_error(f"Failed to create material instance for {fade_material_name}")
        return
    
    # Set the material of the instance to the new fade material
    material_instance.set_editor_property('parent', fade_material)
    
    # Save the new material instance
    unreal.EditorAssetLibrary.save_asset(new_instance_path)
    
    # Log success
    unreal.log(f"Created material instance: {new_instance_path}")
    
# Example usage: Process selected material assets
def process_selected_assets(fade_param_name):
    unreal.log_warning(f"Attempting to create fade material for selected assets")
    selected_assets = unreal.EditorUtilityLibrary.get_selected_assets()
    for asset in selected_assets:
        unreal.log_warning(f"Attempting to create fade material for {asset.get_name()}")
        if isinstance(asset, unreal.Material):
            create_fade_material(asset, fade_param_name)
        else:
            unreal.log_warning(f"Skipped non-material asset: {asset.get_name()}")