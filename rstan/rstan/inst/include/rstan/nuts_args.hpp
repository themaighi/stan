
#ifndef __RSTAN__IO__NUTS_ARGS_HPP__
#define __RSTAN__IO__NUTS_ARGS_HPP__


#include <Rcpp.h>
// #include <R.h>
// #include <Rinternals.h> 

#include <rstan/io/r_ostream.hpp> 

namespace rstan {

  namespace {
    /** 
     * Find the index of an element in a vector. 
     * @param v the vector in which an element are searched. 
     * @param e the element that we are looking for. 
     * @return If e is in v, return the index (0 to size - 1);
     *  otherwise, return the size. 
     */
   
    template <class T>
    size_t find_index(std::vector<T> v, const T& e) {
      return std::distance(v.begin(), std::find(v.begin(), v.end(), e));  
    } 
  } 
  /**
   * Wrap the available arguments for nuts sampler (and other samplers) from
   * Rcpp::List and set the defaults if not available. 
   *
   *
   * The following arguments could be in the names lists
   *
   * <ul>
   * <li> sample_file: into which samples are written (default: samples.csv)
   * <li> iter: total number of iterations, including warmup (default: 2000)  
   * <li> warmup: 
   * <li> thin 
   * <li> refresh 
   * <li> leapfrog_steps
   * <li> epsilon
   * <li> max_treedepth 
   * <li> epsilon_pm
   * <li> epsilon_adapt
   * <li> delta 
   * <li> gamma 
   * <li> random_seed 
   * <li> append_samples 
   * <li> test_grad 
   * <li> init 
   * <li> init_list 
   * <li> num_chains 
   * </ul>
   *
   * In addition, the following keep a record of how the arguments are set: by
   * user or default. 
   * <ul> 
   * <li> random_seed_src 
   * <li> chain_id_src 
   * </ul> 
   *
   */ 
  class nuts_args {
  private:
    std::string sample_file; // the file for outputting the samples 
    unsigned int iter;   // number of iterations 
    unsigned int warmup; // number of warmup 
    unsigned int thin; 
    unsigned int refresh; 
    int leapfrog_steps; 
    double epsilon; 
    int max_treedepth; 
    double epsilon_pm; 
    bool epsilon_adapt; 
    double delta; 
    double gamma; 
    int random_seed; 
    std::string random_seed_src; // "user" or "default" 
    unsigned int chain_id; 
    std::string chain_id_src; // "user" or "default" 
    bool append_samples; 
    bool test_grad; 
    std::string init; 
    SEXP init_list;  
    size_t num_chains;
   
  public:
    /**
    nuts_args(): 
      sample_file("samples.csv"),  
      iter(2000U), 
      warmup(1000U), 
      thin(1U), 
      refresh(1U), 
      leapfrog_steps(-1), 
      epsilon(-1.0), 
      max_treedepth(10), 
      epsilon_pm(0.0), 
      epsilon_adapt(true), 
      delta(0.5), 
      gamma(0.05), 
      random_seed(std::time(0)), 
      random_seed_src("default"), 
      chain_id(1), 
      chain_id_src("default"), 
      append_samples(false), 
      test_grad(true), 
      init("random"),
      init_list(R_NilValue),
      num_chains(1) {
    } 
    */
    nuts_args(const Rcpp::List& in) {
      std::vector<std::string> args_names 
        = Rcpp::as<std::vector<std::string> >(in.names()); 
   
      size_t idx = find_index(args_names, std::string("sample_file")); 
      if (idx == args_names.size()) sample_file = "samples.csv"; 
      else sample_file = Rcpp::as<std::string>(in[idx]); 

      idx = find_index(args_names, std::string("iter")); 
      if (idx == args_names.size()) iter = 2000U;  
      else iter = Rcpp::as<unsigned int>(in[idx]); 

      idx = find_index(args_names, std::string("warmup")); 
      if (idx == args_names.size()) warmup = iter / 2; 
      else warmup = Rcpp::as<unsigned int>(in[idx]); 

      idx = find_index(args_names, std::string("thin")); 
      unsigned int calculated_thin = (iter - warmup) / 1000U;
      if (idx == args_names.size()) thin = (calculated_thin > 1) ? calculated_thin : 1U;
      else thin = Rcpp::as<unsigned int>(in[idx]); 

      idx = find_index(args_names, std::string("leapfrog_steps"));
      if (idx == args_names.size()) leapfrog_steps = -1; 
      else leapfrog_steps = Rcpp::as<int>(in[idx]); 

      idx = find_index(args_names, std::string("epsilon")); 
      if (idx == args_names.size()) epsilon = -1.0; 
      else epsilon = Rcpp::as<double>(in[idx]); 

      idx = find_index(args_names, std::string("epsilon_pm")); 
      if (idx == args_names.size()) epsilon_pm = 0.0; 
      else epsilon_pm = Rcpp::as<double>(in[idx]); 

      idx = find_index(args_names, std::string("max_treedepth")); 
      if (idx == args_names.size())  max_treedepth = 10; 
      else max_treedepth = Rcpp::as<int>(in[idx]); 
     
      idx = find_index(args_names, std::string("epsilon_adapt")); 
      if (idx == args_names.size()) epsilon_adapt = true; 
      else epsilon_adapt = Rcpp::as<bool>(in[idx]); 

      idx = find_index(args_names, std::string("delta")); 
      if (idx == args_names.size())  delta = 0.5;
      else delta = Rcpp::as<double>(in[idx]); 

      idx = find_index(args_names, std::string("gamma")); 
      if (idx == args_names.size()) gamma = 0.05; 
      else gamma = Rcpp::as<double>(in[idx]); 
      
      idx = find_index(args_names, std::string("refresh")); 
      if (idx == args_names.size())  refresh = 1; 
      else refresh = Rcpp::as<unsigned int>(in[idx]); 


      idx = find_index(args_names, std::string("seed")); 
      if (idx == args_names.size()) {
        random_seed = std::time(0); 
        random_seed_src = "random"; 
      } else {
        random_seed = Rcpp::as<unsigned int>(in[idx]); 
        random_seed_src = "user"; 
      }

      idx = find_index(args_names, std::string("chain_id")); 
      if (idx == args_names.size()) { 
        chain_id = 1; 
        chain_id_src = "default"; 
      } else {
        chain_id = Rcpp::as<unsigned int>(in[idx]); 
        chain_id_src = "user"; 
      }
      
      idx = find_index(args_names, std::string("init")); 
      if (idx == args_names.size()) init = "random"; 
      else init = Rcpp::as<std::string>(in[idx]); // "0", "user", or "random"

      idx = find_index(args_names, std::string("init_list")); 
      if (idx == args_names.size()) init_list = R_NilValue; 
      else init_list = in[idx]; 

      // rstan::io::rcout << "init=" << init << std::endl;  
      // std::string yesorno = Rf_isNull(init_list) ? "yes" : "no";
      // rstan::io::rcout << "init_list is null: " << yesorno << std::endl; 

      idx = find_index(args_names, std::string("append_samples")); 
      if (idx == args_names.size()) append_samples = false; 
      else append_samples = Rcpp::as<bool>(in[idx]); 

      idx = find_index(args_names, std::string("num_chains")); 
      if (idx == args_names.size()) num_chains = 1;
      else num_chains = Rcpp::as<size_t>(in[idx]); 

    } 
    size_t get_num_chains() const {
      return num_chains; 
    } 
    const std::string& get_random_seed_src() const {
      return random_seed_src; 
    } 
    const std::string& get_chain_id_src() const {
      return chain_id_src; 
    } 

    SEXP get_init_list() const {
      return init_list; 
    } 
    int get_iter() const {
      return iter; 
    } 
    const std::string& get_sample_file() const {
      return sample_file; 
    } 
    unsigned int get_warmup() const {
      return warmup; 
    } 
    unsigned int get_refresh() const { 
      return refresh; 
    } 
    unsigned int get_thin() const {
      return thin;
    } 
    int get_leapfrog_steps() const {
      return leapfrog_steps; 
    } 
    double get_epsilon() const {
      return epsilon; 
    } 
    int get_max_treedepth() const {
      return max_treedepth; 
    } 
    double get_epsilon_pm() const {
      return epsilon; 
    } 
    bool get_epsilon_adapt() const {
      return epsilon_adapt; 
    } 
    double get_delta() const {  
      return delta;
    } 
    double get_gamma() const { 
      return gamma;
    } 
    bool get_append_samples() const {
      return append_samples; 
    } 
    bool get_test_grad() const {
      return test_grad; 
    } 
    int get_random_seed() const {
      return random_seed; 
    } 
    const std::string& get_init() const {
      return init;
    } 
    unsigned int get_chain_id() const {
      return chain_id; 
    } 
  }; 
} 

#endif 

