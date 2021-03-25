#-------------------------------------------------------------------------------

""" Storage classes that can be used by user to further analyse fit results.
"""

#-------------------------------------------------------------------------------

# External packages
import numpy as np

#-------------------------------------------------------------------------------
  
class FitResult:
  """ Storage class for results of an individual fit.
  """
  def __init__(self):
    self.pars_fin   = np.array([]) # Final parameter values after fit
    self.uncs_fin   = np.array([]) # Final parameter uncertainties after fit
    self.cov_matrix = np.array([[]]) # Covariance matrix after fit
    self.cor_matrix = np.array([[]]) # Correlation matrix after fit
    self.n_bins = -1;      # Number of bins that the minimization was performed on
    self.n_free_pars = -1; # Number of non-fixed parameters
    self.n_fct_calls = -1; # Number of chi^2-function calls by minimizer
    self.n_iters = -1;     # Number of iterations in minimization stepping
    self.chisq_fin  = -1 # Chi-Squared at fit result
    self.edm_fin    = 0  # Expected distance from minimum at fit result
    self.min_status = -1 # Status of minimization (see Minuit2, 0=success)
    self.cov_status = -1 # Status of covariance matrix calculation (see Minuit2)
  
#-------------------------------------------------------------------------------
  
class RunResult:
  """ Storage class for results of a whole run of potentially multiple fits.
  """
  def __init__(self):
    self.par_names = []  # Names of parameters used in fit
    self.fit_results = []  # Results of individual fits performed in this setup

#-------------------------------------------------------------------------------