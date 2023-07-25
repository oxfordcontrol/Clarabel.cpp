#pragma once

#include <cstdint>
#include <type_traits>

namespace clarabel
{
    enum class ClarabelDirectSolveMethods
    {
        QDLDL,
        // MKL, (not supported in Rust yet)
        // CHOLMOD, (not supported in Rust yet)
    };

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
    };

    class DefaultSettingsBuilder
    {
    public:
        template<typename T = double>
        static DefaultSettings<T> default_settings();

        // TODO: builder pattern support for DefaultSettings
    };

    extern "C"
    {
        DefaultSettings<double> clarabel_DefaultSettingsBuilder_f64_default();
        DefaultSettings<float> clarabel_DefaultSettingsBuilder_f32_default();
    }

    template<>
    inline DefaultSettings<double> DefaultSettingsBuilder::default_settings<double>()
    {
        return clarabel_DefaultSettingsBuilder_f64_default();
    }

    template<>
    inline DefaultSettings<float> DefaultSettingsBuilder::default_settings<float>()
    {
        return clarabel_DefaultSettingsBuilder_f32_default();
    }
}