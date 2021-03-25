#-------------------------------------------------------------------------------

""" Reader classes to interpret the output of a PrEW run.
"""

#-------------------------------------------------------------------------------

# Local modules
import Markers # PrEW output markers
import Results # PrEW result storage classes

#-------------------------------------------------------------------------------

# External packages
import numpy as np

#-------------------------------------------------------------------------------

class RunReader:
  """ Class to read one PrEW run (which can contain many fits in the same 
      setup).
  """
  def __init__(self,run_lines):
    """ Class takes array of string lines representing the output of one PrEW 
        run.
    """
    self.run_lines = run_lines
    self.run_result = Results.RunResult() # The final result
        
  def find_fits(self):
    """ Find the string lines of the run which represent the individual fit 
        results. 
    """
    fits = []
    found_fits = False
    for line in self.run_lines:
      if line == Markers.fits_beg:
        # Found the beginning marker, start taking lines from here
        found_fits = True
        continue
      elif line == Markers.fits_end:
        # Found the ending marker, stop taking lines
        break
      elif found_fits == True:
        # If beginning marker already found use this line
        fits.append(line)
    return fits
    
  def add_fit_result(self,fit_result_lines):
    """ Interpret the lines that describe one individual fit result and store 
        the corresponding values.
    """
    fit_result = Results.FitResult() # Class object decribing the resulting values
    
    for l in range(len(fit_result_lines)):
      split_line = fit_result_lines[l].split(" ")
      if len(split_line) == 0 : continue # Ignore empty lines
      
      if (split_line[0] == "Fin:"):
        # Found final parameter values after fit
        fit_result.pars_fin = np.array([float(val) for val in split_line[1:]])
      elif (split_line[0] == "Unc:"):
        # Found final uncertainties after fit
        fit_result.uncs_fin = np.array([float(val) for val in split_line[1:]])
      elif (split_line[0] == "Cov:"):
        # Found covariance matrix after fit
        cov_matrix = []
        # Read the nxn (n=#parameters) matrix describing the covariance matrix
        for p in range(len(self.run_result.par_names)):
          cov_line = fit_result_lines[l+p+1].split(" ")
          matrix_line = np.array([float(val) for val in cov_line])
          if len(cov_matrix) == 0: cov_matrix = matrix_line
          else: cov_matrix = np.vstack((cov_matrix,matrix_line))
        fit_result.cov_matrix = cov_matrix
      elif (split_line[0] == "Cor:"):
        # Found correlation matrix after fit
        cor_matrix = []
        # Read the nxn (n=#parameters) matrix describing the correlation matrix
        for p in range(len(self.run_result.par_names)):
          cor_line = fit_result_lines[l+p+1].split(" ")
          matrix_line = np.array([float(val) for val in cor_line])
          if len(cor_matrix) == 0: cor_matrix = matrix_line
          else: cor_matrix = np.vstack((cor_matrix,matrix_line))
        fit_result.cor_matrix = cor_matrix
      elif (split_line[0] == "NBins:"):
        # Found line describing the number of input bins
        fit_result.n_bins = int(split_line[1])
      elif (split_line[0] == "NFreePars:"):
        # Found line describing the number of free parameters
        fit_result.n_free_pars = int(split_line[1])
      elif (split_line[0] == "NFctCalls:"):
        # Found line describing the number of function calls
        fit_result.n_fct_calls = int(split_line[1])
      elif (split_line[0] == "NIterations:"):
        # Found line describing the number of iterations
        fit_result.n_iters = int(split_line[1])
      elif (split_line[0] == "Chi-Sq:"):
        # Found the line describing the final chi^2 value
        fit_result.chisq_fin = float(split_line[1])
      elif (split_line[0] == "EDM:"):
        # Found the line describing the final expected distance to the minimum
        fit_result.edm_fin = float(split_line[1])
      elif (split_line[0] == "Min.-Status:"):
        # Found line describing minimzer status
        fit_result.min_status = int(split_line[1])
      elif (split_line[0] == "Cov.-Status:"):
        # Found the line with the status of the Minuit2 cov. matrix calculation
        fit_result.cov_status = int(split_line[1])
      
    # Add the result of this individual fit to the result of the complete run
    self.run_result.fit_results.append(fit_result)
    
  def read_fits(self,fits):
    """ Read and interpret the lines describing the results of the individual 
        fits.
    """
    current_fit_result = []
    for line in fits:
      split_line = line.split(" ")
      if (len(split_line) > 0) and (split_line[0] == "Parameters:"):
        # Found the line that has the parameter names
        self.run_result.par_names = [par for par in split_line[1:]]
      elif Markers.is_fit_id_beg(line):
        # Found the beginning of one individual fit result
        current_fit_result = []
      elif Markers.is_fit_id_end(line):
        # Found the end of this individual result -> Interpret it
        self.add_fit_result(current_fit_result)
      else:
        # Line belongs to the current fit result
        current_fit_result.append(line)
      
  def interpret(self):
    """ Interpret the lines provided in the constructor and return the 
        corresponding fit result class object.
    """
    self.read_fits(self.find_fits())
    return self.run_result

#-------------------------------------------------------------------------------

class Reader:
  """ Interface class to read a PrEW output file.
      Call simply using 
        reader = Reader(file_path)
        reader.read()
        results = reader.run_results
      Return array of RunResult objects which contain the information which was 
      output by PrEW.
  """
  
  def __init__(self,file_path):
    """ Constructor take the PrEW output file path
    """
    self.file_path = file_path
    self.lines = [] # Line from input file
    self.run_results = [] # Results after reading the file
    
  def identify_runs(self):
    """ Look through the lines and identify individual PrEW runs.
        Such runs are fully independent but can be stored in the same file.
    """
    runs = []
    current_run = []
    found_run = False
    for line in self.lines:
      line = line.strip() # Remove trailing/leading whitespaces etc.
      if line == Markers.run_sep_beg:
        # Found beginning of run -> Start reading lines
        found_run = True
      elif (line == Markers.run_sep_end) and (current_run != []):
        # Found end of one run, don't read lines unless new one found
        runs.append(current_run)
        found_run = False
      elif found_run:
        # Append line to current run
        current_run.append(line)
    return runs

  def read(self):
    """ Read and interpret the line of the input file.
    """
    with open(self.file_path) as f:
      self.lines = f.readlines() # list containing lines of file

    runs = self.identify_runs()
    for run in runs:
      run_reader = RunReader(run)
      self.run_results.append(run_reader.interpret())

#-------------------------------------------------------------------------------