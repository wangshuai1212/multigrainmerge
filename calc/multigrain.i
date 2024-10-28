#
# simulate how the grain boundary between two grains moves 
#

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 200
  ny = 100
  xmin = 0
  xmax = 20000
  ymin = 0
  ymax = 11000
  elem_type = QUAD4
[]

[GlobalParams]
  op_num = 8
  var_name_base = gr
  liquid =1
[]

[Functions]
  [./TGradient]
    type = ParsedFunction
    #expression = '1800'
    expression = '1600 + (2000-1600)*x/20000'
  [../]
[]

[Variables]
  [PolycrystalVariables]
    order = FIRST
    family = LAGRANGE
  []

  [./rho]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = SmoothCircleIC
      x1 = 10000
      y1 = 5500
      radius = 5000
      invalue = 1
      outvalue = 0
      int_width = 400
    [../]
  [../]
  [./w]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[UserObjects]
  [voronoi]
    type = PolycrystalVoronoi
    grain_num = 50
    rand_seed = 80
    roloring_algorithm = bt
  []
[]

[ICs]
  [./PolycrystalICs]
    [./PolycrystalColoringIC]
      polycrystal_ic_uo = voronoi
    [../]
  [../]
[]

[AuxVariables]
  [./T]
  [../]

  [bnds]
    order = FIRST
    family = LAGRANGE
  []
[]

[AuxKernels]
  [./Tgrad]
    type = FunctionAux
    variable = T
    function = TGradient
  [../]

  [./bnds_aux]
    type = BndsCalcAux
    variable = bnds
    execute_on = TIMESTEP_BEGIN
  [../]
[]



[Kernels]

  
[LoopACGrGrPoly]
  variable_mobility = true
  coupled_variables = 'T'
  
  
[]
  [./rho_res]
    type = SplitCHParsed
    variable = rho
    f_name = F
    kappa_name = kappa_c
    w = w
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = rho
  [../]
[]

[Materials]
  [./consts]
    type = GenericConstantMaterial
    prop_names = 'kappa_c M'
    prop_values = '25000 50000'
  [../]

  [./SiC]
    type = GBEvolution
    GBmob0 = 4.36e-6
    GBenergy = 2.5
    Q = 1.7
    T = T
    wGB = 1250
  [../]

  [./free_energy]
    type = DerivativeParsedMaterial
    property_name = F
    coupled_variables = 'rho gr0 gr1 gr2 gr3 gr4 gr5 gr6 gr7'
    constant_names = 'Wo Wd'
    constant_expressions = '1 2' 
#    expression = 'rho*0.015*(eta0^4/4-eta0^2/2+eta1^4/4-eta1^2/2+2*1.5*eta0^2*eta1^2+0.5)'
#    expression = 'rho*(4/3*(1-4*(eta0^3+eta1^3)+3*(eta0^2+eta1^2)^2))'
#    expression = 'Wo*rho^2*(1-rho)^2 + Wd*(rho^2+6*(1-rho)*(eta0^2+eta1^2)-4*(2-rho)*(eta0^3+eta1^3)+3*(eta0^2+eta1^2)^2)'
    expression = 'Wo*rho^2*(1-rho)^2 + Wd*(rho^2+6*(1-rho)*(gr0^2+gr1^2+gr2^2+gr3^2+gr4^2+gr5^2+gr6^2+gr7^2)
                  -4*(2-rho)*(gr0^3+gr1^3+gr2^3+gr3^3+gr4^3+gr5^3+gr6^3+gr7^3)+3*(gr0^2+gr1^2+gr2^2+gr3^2+gr4^2+gr5^2+gr6^2+gr7^2)^2)'
    derivative_order = 2
  [../]

  [./mobility_L]
    type = ParsedMaterial
    property_name = Mo
    coupled_variables = 'rho T'
    expression = '3e8'
  [../]
[]

[Preconditioning]
  # activate = ''
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_shift_type'
  petsc_options_value = 'asm      ilu          nonzero'

  l_max_its = 30
  nl_max_its = 30
  l_tol = 1.0e-4
  nl_rel_tol = 1.0e-10
  nl_abs_tol = 1.0e-11
  start_time = 0.0
  num_steps = 10
#  end_time = 500.0

#  dt = 1

  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1
    optimal_iterations = 10
  [../]
[]



#[Debug]
#  show_actions = true
#  show_action_dependencies = true
#[]

[Outputs]
  exodus = true
[]
