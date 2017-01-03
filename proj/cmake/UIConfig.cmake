if( NOT TARGET UI )
	get_filename_component( UI_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE )
  get_filename_component( CINDER_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../.." ABSOLUTE )

	list( APPEND UI_SOURCES
    ${UI_SOURCE_PATH}/BSplineEditor.cpp
    ${UI_SOURCE_PATH}/Button.cpp
    ${UI_SOURCE_PATH}/Canvas.cpp
    ${UI_SOURCE_PATH}/ColorPicker.cpp
    ${UI_SOURCE_PATH}/Colors.cpp
    ${UI_SOURCE_PATH}/Control.cpp
    ${UI_SOURCE_PATH}/ControlWithLabel.cpp
    ${UI_SOURCE_PATH}/Dialer.cpp
    ${UI_SOURCE_PATH}/Envelope.cpp
    ${UI_SOURCE_PATH}/Fps.cpp
    ${UI_SOURCE_PATH}/Globals.cpp
    ${UI_SOURCE_PATH}/Graph.cpp
    ${UI_SOURCE_PATH}/Label.cpp
    ${UI_SOURCE_PATH}/MultiSlider.cpp
    ${UI_SOURCE_PATH}/Plotter.cpp
    ${UI_SOURCE_PATH}/QuaternionOrderer.cpp
    ${UI_SOURCE_PATH}/Radio.cpp
    ${UI_SOURCE_PATH}/Range.cpp
    ${UI_SOURCE_PATH}/Responder.cpp
    ${UI_SOURCE_PATH}/Slider.cpp
    ${UI_SOURCE_PATH}/Spacer.cpp
    ${UI_SOURCE_PATH}/Spectrum.cpp
    ${UI_SOURCE_PATH}/SuperCanvas.cpp
    ${UI_SOURCE_PATH}/TextInput.cpp
    ${UI_SOURCE_PATH}/TextureView.cpp
    ${UI_SOURCE_PATH}/Toggle.cpp
    ${UI_SOURCE_PATH}/View.cpp
    ${UI_SOURCE_PATH}/Waveform.cpp
    ${UI_SOURCE_PATH}/WindowCanvas.cpp
    ${UI_SOURCE_PATH}/XYPad.cpp
	)

	add_library( UI ${UI_SOURCES} )

	target_include_directories( UI PUBLIC "${UI_SOURCE_PATH}" )
  target_include_directories( UI SYSTEM BEFORE PUBLIC "${CINDER_PATH}/include" )

  if( NOT TARGET cinder )
		    include( "${CINDER_PATH}/proj/cmake/configure.cmake" )
		    find_package( cinder REQUIRED PATHS
		        "${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
		        "$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}" )
	endif()
	target_link_libraries( UI PRIVATE cinder )

endif()
