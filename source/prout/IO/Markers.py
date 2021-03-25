#-------------------------------------------------------------------------------

""" Markers used in the output of PrEW.
    Also includes helper functions to check matching to these markers.
"""

#-------------------------------------------------------------------------------

run_sep_beg = "<=========================== BEGIN ==========================>"
run_sep_end = "<============================ END ===========================>"
setup_beg = "<SETUP>"
setup_end = "<END SETUP>"
fits_beg = "<FITS>"
fits_end = "<END FITS>"
fit_id_beg = "[F"
fit_id_end = "[END F"
fit_id_cls = "]"

#-------------------------------------------------------------------------------
  
def is_fit_id_beg(str):
  """ Check if string fits shape of fit beginning identifier. """
  return str.startswith(fit_id_beg) and str.endswith(fit_id_cls)

def is_fit_id_end(str):
  """ Check if string fits shape of fit end identifier. """
  return str.startswith(fit_id_end) and str.endswith(fit_id_cls)

#-------------------------------------------------------------------------------