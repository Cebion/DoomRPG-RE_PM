find_path(FLUIDSYNTH_INCLUDE_DIR fluidsynth.h
  HINTS
  /usr/include
  /usr/local/include
)

find_library(FLUIDSYNTH_LIBRARY
  NAMES fluidsynth libfluidsynth
  HINTS
  /usr/lib
  /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FluidSynth DEFAULT_MSG FLUIDSYNTH_LIBRARY FLUIDSYNTH_INCLUDE_DIR)

mark_as_advanced(FLUIDSYNTH_INCLUDE_DIR FLUIDSYNTH_LIBRARY)

if(FluidSynth_FOUND AND NOT TARGET FluidSynth::libfluidsynth)
  add_library(FluidSynth::libfluidsynth UNKNOWN IMPORTED)
  set_target_properties(FluidSynth::libfluidsynth PROPERTIES
    IMPORTED_LOCATION "${FLUIDSYNTH_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${FLUIDSYNTH_INCLUDE_DIR}"
  )
endif()
