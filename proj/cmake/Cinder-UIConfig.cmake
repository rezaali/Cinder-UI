if( NOT TARGET Cinder-UI )
	get_filename_component( CINDER_UI_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE )

	list( APPEND CINDER_UI_SOURCES
		${CINDER_UI_SOURCE_PATH}/BSplineEditor.cpp
		${CINDER_UI_SOURCE_PATH}/Button.cpp
		${CINDER_UI_SOURCE_PATH}/Canvas.cpp
		${CINDER_UI_SOURCE_PATH}/ColorPicker.cpp
		${CINDER_UI_SOURCE_PATH}/Colors.cpp
		${CINDER_UI_SOURCE_PATH}/Control.cpp
		${CINDER_UI_SOURCE_PATH}/ControlWithLabel.cpp
		${CINDER_UI_SOURCE_PATH}/Dialer.cpp
		${CINDER_UI_SOURCE_PATH}/Envelope.cpp
		${CINDER_UI_SOURCE_PATH}/Fps.cpp
		${CINDER_UI_SOURCE_PATH}/Globals.cpp
		${CINDER_UI_SOURCE_PATH}/Graph.cpp
		${CINDER_UI_SOURCE_PATH}/Label.cpp
		${CINDER_UI_SOURCE_PATH}/MultiSlider.cpp
		${CINDER_UI_SOURCE_PATH}/Plotter.cpp
		${CINDER_UI_SOURCE_PATH}/QuaternionOrderer.cpp
		${CINDER_UI_SOURCE_PATH}/Radio.cpp
		${CINDER_UI_SOURCE_PATH}/Range.cpp
		${CINDER_UI_SOURCE_PATH}/Responder.cpp
		${CINDER_UI_SOURCE_PATH}/Slider.cpp
		${CINDER_UI_SOURCE_PATH}/Spacer.cpp
		${CINDER_UI_SOURCE_PATH}/Spectrum.cpp
		${CINDER_UI_SOURCE_PATH}/SuperCanvas.cpp
		${CINDER_UI_SOURCE_PATH}/TextInput.cpp
		${CINDER_UI_SOURCE_PATH}/TextureView.cpp
		${CINDER_UI_SOURCE_PATH}/Toggle.cpp
		${CINDER_UI_SOURCE_PATH}/View.cpp
		${CINDER_UI_SOURCE_PATH}/Waveform.cpp
		${CINDER_UI_SOURCE_PATH}/WindowCanvas.cpp
		${CINDER_UI_SOURCE_PATH}/XYPad.cpp
	)

	add_library( Cinder-UI ${CINDER_UI_SOURCES} )

	target_include_directories( Cinder-UI PUBLIC "${CINDER_UI_SOURCE_PATH}" )

	if( NOT TARGET cinder )
	    include( "${CINDER_PATH}/proj/cmake/configure.cmake" )
	    find_package( cinder REQUIRED PATHS
	        "${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
	        "$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}" )
	endif()
	target_link_libraries( Cinder-UI PRIVATE cinder )
endif()
