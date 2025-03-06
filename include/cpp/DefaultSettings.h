#pragma once

#include <cstdint>
#include <type_traits>

namespace clarabel
{

enum class ClarabelDirectSolveMethods
{
    AUTO,
    QDLDL,
    #ifdef FEATURE_FAER_SPARSE
    FAER,
    #endif 
    // MKL, (not supported in Rust yet)
    // CHOLMOD, (not supported in Rust yet)
};

#ifdef FEATURE_SDP
    enum class ClarabelCliqueMergeMethods
    {
        CLIQUE_GRAPH,
        PARENT_CHILD,
        NONE,
    };
#endif 

template<typename T = double>
struct DefaultSettings
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

    uint32_t max_iter;
    double time_limit;
    bool verbose;
    T max_step_fraction;
    T tol_gap_abs;
    T tol_gap_rel;
    T tol_feas;
    T tol_infeas_abs;
    T tol_infeas_rel;
    T tol_ktratio;
    T reduced_tol_gap_abs;
    T reduced_tol_gap_rel;
    T reduced_tol_feas;
    T reduced_tol_infeas_abs;
    T reduced_tol_infeas_rel;
    T reduced_tol_ktratio;
    bool equilibrate_enable;
    uint32_t equilibrate_max_iter;
    T equilibrate_min_scaling;
    T equilibrate_max_scaling;
    T linesearch_backtrack_step;
    T min_switch_step_length;
    T min_terminate_step_length;
    uint32_t max_threads;
    bool direct_kkt_solver;
    ClarabelDirectSolveMethods direct_solve_method;
    bool static_regularization_enable;
    T static_regularization_constant;
    T static_regularization_proportional;
    bool dynamic_regularization_enable;
    T dynamic_regularization_eps;
    T dynamic_regularization_delta;
    bool iterative_refinement_enable;
    T iterative_refinement_reltol;
    T iterative_refinement_abstol;
    uint32_t iterative_refinement_max_iter;
    T iterative_refinement_stop_ratio;
    bool presolve_enable;
    #ifdef FEATURE_SDP
        bool chordal_decomposition_enable;
        ClarabelCliqueMergeMethods chordal_decomposition_merge_method;
        bool chordal_decomposition_compact;
        bool chordal_decomposition_complete_dual;
    #endif
    

    static DefaultSettings<T> default_settings();
};

// Instantiate the templates
template struct DefaultSettings<double>;
template struct DefaultSettings<float>;

template<typename T = double>
class DefaultSettingsBuilder
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

  private:
    DefaultSettings<T> settings;
    DefaultSettingsBuilder() { settings = DefaultSettings<T>::default_settings(); }

  public:
    static DefaultSettingsBuilder<T> default_settings() { return DefaultSettingsBuilder<T>(); }

    DefaultSettings<T> build() { return settings; }

    DefaultSettingsBuilder<T> &max_iter(uint32_t max_iter)
    {
        settings.max_iter = max_iter;
        return *this;
    }

    DefaultSettingsBuilder<T> &time_limit(double time_limit)
    {
        settings.time_limit = time_limit;
        return *this;
    }

    DefaultSettingsBuilder<T> &verbose(bool verbose)
    {
        settings.verbose = verbose;
        return *this;
    }

    DefaultSettingsBuilder<T> &max_step_fraction(T max_step_fraction)
    {
        settings.max_step_fraction = max_step_fraction;
        return *this;
    }

    DefaultSettingsBuilder<T> &tol_gap_abs(T tol_gap_abs)
    {
        settings.tol_gap_abs = tol_gap_abs;
        return *this;
    }

    DefaultSettingsBuilder<T> &tol_gap_rel(T tol_gap_rel)
    {
        settings.tol_gap_rel = tol_gap_rel;
        return *this;
    }

    DefaultSettingsBuilder<T> &tol_feas(T tol_feas)
    {
        settings.tol_feas = tol_feas;
        return *this;
    }

    DefaultSettingsBuilder<T> &tol_infeas_abs(T tol_infeas_abs)
    {
        settings.tol_infeas_abs = tol_infeas_abs;
        return *this;
    }

    DefaultSettingsBuilder<T> &tol_infeas_rel(T tol_infeas_rel)
    {
        settings.tol_infeas_rel = tol_infeas_rel;
        return *this;
    }

    DefaultSettingsBuilder<T> &tol_ktratio(T tol_ktratio)
    {
        settings.tol_ktratio = tol_ktratio;
        return *this;
    }

    DefaultSettingsBuilder<T> &reduced_tol_gap_abs(T reduced_tol_gap_abs)
    {
        settings.reduced_tol_gap_abs = reduced_tol_gap_abs;
        return *this;
    }

    DefaultSettingsBuilder<T> &reduced_tol_gap_rel(T reduced_tol_gap_rel)
    {
        settings.reduced_tol_gap_rel = reduced_tol_gap_rel;
        return *this;
    }

    DefaultSettingsBuilder<T> &reduced_tol_feas(T reduced_tol_feas)
    {
        settings.reduced_tol_feas = reduced_tol_feas;
        return *this;
    }

    DefaultSettingsBuilder<T> &reduced_tol_infeas_abs(T reduced_tol_infeas_abs)
    {
        settings.reduced_tol_infeas_abs = reduced_tol_infeas_abs;
        return *this;
    }

    DefaultSettingsBuilder<T> &reduced_tol_infeas_rel(T reduced_tol_infeas_rel)
    {
        settings.reduced_tol_infeas_rel = reduced_tol_infeas_rel;
        return *this;
    }

    DefaultSettingsBuilder<T> &reduced_tol_ktratio(T reduced_tol_ktratio)
    {
        settings.reduced_tol_ktratio = reduced_tol_ktratio;
        return *this;
    }

    DefaultSettingsBuilder<T> &equilibrate_enable(bool equilibrate_enable)
    {
        settings.equilibrate_enable = equilibrate_enable;
        return *this;
    }

    DefaultSettingsBuilder<T> &equilibrate_max_iter(uint32_t equilibrate_max_iter)
    {
        settings.equilibrate_max_iter = equilibrate_max_iter;
        return *this;
    }

    DefaultSettingsBuilder<T> &equilibrate_min_scaling(T equilibrate_min_scaling)
    {
        settings.equilibrate_min_scaling = equilibrate_min_scaling;
        return *this;
    }

    DefaultSettingsBuilder<T> &equilibrate_max_scaling(T equilibrate_max_scaling)
    {
        settings.equilibrate_max_scaling = equilibrate_max_scaling;
        return *this;
    }

    DefaultSettingsBuilder<T> &linesearch_backtrack_step(T linesearch_backtrack_step)
    {
        settings.linesearch_backtrack_step = linesearch_backtrack_step;
        return *this;
    }

    DefaultSettingsBuilder<T> &min_switch_step_length(T min_switch_step_length)
    {
        settings.min_switch_step_length = min_switch_step_length;
        return *this;
    }

    DefaultSettingsBuilder<T> &min_terminate_step_length(T min_terminate_step_length)
    {
        settings.min_terminate_step_length = min_terminate_step_length;
        return *this;
    }

    DefaultSettingsBuilder<T> &max_threads(uint32_t max_threads)
    {
        settings.max_threads = max_threads;
        return *this;
    }

    DefaultSettingsBuilder<T> &direct_kkt_solver(bool direct_kkt_solver)
    {
        settings.direct_kkt_solver = direct_kkt_solver;
        return *this;
    }

    DefaultSettingsBuilder<T> &direct_solve_method(ClarabelDirectSolveMethods direct_solve_method)
    {
        settings.direct_solve_method = direct_solve_method;
        return *this;
    }

    DefaultSettingsBuilder<T> &static_regularization_enable(bool static_regularization_enable)
    {
        settings.static_regularization_enable = static_regularization_enable;
        return *this;
    }

    DefaultSettingsBuilder<T> &static_regularization_constant(T static_regularization_constant)
    {
        settings.static_regularization_constant = static_regularization_constant;
        return *this;
    }

    DefaultSettingsBuilder<T> &static_regularization_proportional(T static_regularization_proportional)
    {
        settings.static_regularization_proportional = static_regularization_proportional;
        return *this;
    }

    DefaultSettingsBuilder<T> &dynamic_regularization_enable(bool dynamic_regularization_enable)
    {
        settings.dynamic_regularization_enable = dynamic_regularization_enable;
        return *this;
    }

    DefaultSettingsBuilder<T> &dynamic_regularization_eps(T dynamic_regularization_eps)
    {
        settings.dynamic_regularization_eps = dynamic_regularization_eps;
        return *this;
    }

    DefaultSettingsBuilder<T> &dynamic_regularization_delta(T dynamic_regularization_delta)
    {
        settings.dynamic_regularization_delta = dynamic_regularization_delta;
        return *this;
    }

    DefaultSettingsBuilder<T> &iterative_refinement_enable(bool iterative_refinement_enable)
    {
        settings.iterative_refinement_enable = iterative_refinement_enable;
        return *this;
    }

    DefaultSettingsBuilder<T> &iterative_refinement_reltol(T iterative_refinement_reltol)
    {
        settings.iterative_refinement_reltol = iterative_refinement_reltol;
        return *this;
    }

    DefaultSettingsBuilder<T> &iterative_refinement_abstol(T iterative_refinement_abstol)
    {
        settings.iterative_refinement_abstol = iterative_refinement_abstol;
        return *this;
    }

    DefaultSettingsBuilder<T> &iterative_refinement_max_iter(uint32_t iterative_refinement_max_iter)
    {
        settings.iterative_refinement_max_iter = iterative_refinement_max_iter;
        return *this;
    }

    DefaultSettingsBuilder<T> &iterative_refinement_stop_ratio(T iterative_refinement_stop_ratio)
    {
        settings.iterative_refinement_stop_ratio = iterative_refinement_stop_ratio;
        return *this;
    }

    DefaultSettingsBuilder<T> &presolve_enable(bool presolve_enable)
    {
        settings.presolve_enable = presolve_enable;
        return *this;
    }

    #ifdef FEATURE_SDP
        DefaultSettingsBuilder<T> &chordal_decomposition_enable(bool chordal_decomposition_enable)
        {
            settings.chordal_decomposition_enable = chordal_decomposition_enable;
            return *this;
        }

        DefaultSettingsBuilder<T> &chordal_decomposition_merge_method(ClarabelCliqueMergeMethods chordal_decomposition_merge_method)
        {
            settings.chordal_decomposition_merge_method = chordal_decomposition_merge_method;
            return *this;
        }

        DefaultSettingsBuilder<T> &chordal_decomposition_compact(bool chordal_decomposition_compact)
        {
            settings.chordal_decomposition_compact = chordal_decomposition_compact;
            return *this;
        }

        DefaultSettingsBuilder<T> &chordal_decomposition_complete_dual(bool chordal_decomposition_complete_dual)
        {
            settings.chordal_decomposition_complete_dual = chordal_decomposition_complete_dual;
            return *this;
        }
    #endif

};

extern "C" {
DefaultSettings<double> clarabel_DefaultSettings_f64_default();
DefaultSettings<float> clarabel_DefaultSettings_f32_default();
}

template<>
inline DefaultSettings<double> DefaultSettings<double>::default_settings()
{
    return clarabel_DefaultSettings_f64_default();
}

template<>
inline DefaultSettings<float> DefaultSettings<float>::default_settings()
{
    return clarabel_DefaultSettings_f32_default();
}

} // namespace clarabel