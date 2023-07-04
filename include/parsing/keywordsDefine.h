//“—≤π≥‰÷¡2012
#define KEYWORDS_1364_1995 \
    { "always", TokenKind::AlwaysKeyword },\
    { "and", TokenKind::AndKeyword },\
    { "assign", TokenKind::AssignKeyword },\
    { "begin", TokenKind::BeginKeyword },\
    { "buf", TokenKind::BufKeyword },\
    { "case", TokenKind::CaseKeyword },\
    { "default", TokenKind::DefaultKeyword },\
    { "else", TokenKind::ElseKeyword },\
    { "end", TokenKind::EndKeyword },\
    { "for", TokenKind::ForKeyword },\
    { "function", TokenKind::FunctionKeyword },\
    { "if", TokenKind::IfKeyword },\
    { "nor", TokenKind::NorKeyword },\
    { "not", TokenKind::NotKeyword },\
    { "or", TokenKind::OrKeyword },\
    { "while", TokenKind::WhileKeyword },\
    { "unsigned", TokenKind::UnsignedKeyword },\
    { "include", TokenKind::IncludeKeyword },\
    { "byte", TokenKind::ByteKeyword },\
    { "class", TokenKind::ClassKeyword },\
    { "const", TokenKind::ConstKeyword },\
    { "continue", TokenKind::ContinueKeyword },\
    { "enum", TokenKind::EnumKeyword },\
    { "expect", TokenKind::ExpectKeyword },\
    { "extern", TokenKind::ExternKeyword },\
    { "final", TokenKind::FinalKeyword },\
    { "foreach", TokenKind::ForeachKeyword },\
    { "import", TokenKind::ImportKeyword },\
    { "int", TokenKind::IntKeyword },\
    { "new", TokenKind::NewKeyword },\
    { "null", TokenKind::NullKeyword },\
    { "protect", TokenKind::ProtectKeyword },\
    { "rand", TokenKind::RandKeyword },\
    { "random", TokenKind::RandomKeyword },\
    { "return", TokenKind::ReturnKeyword },\
    { "static", TokenKind::StaticKeyword },\
    { "string", TokenKind::StringKeyword },\
    { "struct", TokenKind::StructKeyword },\
    { "this", TokenKind::ThisKeyword },\
    { "type", TokenKind::TypeKeyword },\
    { "typedef", TokenKind::TypedefKeyword },\
    { "union", TokenKind::UnionKeyword },\
    { "unique", TokenKind::UniqueKeyword },\
    { "virtual", TokenKind::VirtualKeyword },\
    { "void", TokenKind::VoidKeyword },\
    {"alignas", TokenKind::AlignasKeyword },\
    {"alignof", TokenKind::AlignofKeyword },\
    {"and", TokenKind::AndKeyword },\
    {"and_eq", TokenKind::And_eqKeyword },\
    {"asm", TokenKind::AsmKeyword },\
    {"atomic_cancel ", TokenKind::Atomic_cancelKeyword },\
    {"atomic_commit ", TokenKind::Atomic_commitKeyword },\
    {"atomic_noexcept ", TokenKind::Atomic_noexceptKeyword },\
    {"auto", TokenKind::AutoKeyword },\
    {"bitand", TokenKind::BitandKeyword },\
    {"bitor", TokenKind::BitorKeyword },\
    {"bool", TokenKind::BoolKeyword },\
    {"break", TokenKind::BreakKeyword },\
    {"case", TokenKind::CaseKeyword },\
    {"catch", TokenKind::CatchKeyword },\
    {"char", TokenKind::CharKeyword },\
    {"char8_t ", TokenKind::Char8_tKeyword },\
    {"char16_t", TokenKind::Char16_tKeyword },\
    {"char32_t", TokenKind::Char32_tKeyword },\
    {"class", TokenKind::ClassKeyword },\
    {"co_await ", TokenKind::Co_awaitKeyword },\
    {"co_return ", TokenKind::Co_returnKeyword },\
    {"co_yield ", TokenKind::Co_yieldKeyword },\
    {"complement ", TokenKind::ComplementKeyword },\
    {"concept ", TokenKind::ConceptKeyword },\
    {"const", TokenKind::ConstKeyword },\
    {"consteval ", TokenKind::ConstevalKeyword },\
    {"constexpr", TokenKind::ConstexprKeyword },\
    {"const_cast", TokenKind::Const_castKeyword },\
    {"continue", TokenKind::ContinueKeyword },\
    {"decltype", TokenKind::DecltypeKeyword },\
    {"default", TokenKind::DefaultKeyword },\
    {"delete", TokenKind::DeleteKeyword },\
    {"do", TokenKind::DoKeyword },\
    {"double", TokenKind::DoubleKeyword },\
    {"dynamic_cast", TokenKind::Dynamic_castKeyword },\
    {"else", TokenKind::ElseKeyword },\
    {"enum", TokenKind::EnumKeyword },\
    {"explicit", TokenKind::ExplicitKeyword },\
    {"export", TokenKind::ExportKeyword },\
    {"extern", TokenKind::ExternKeyword },\
    {"false", TokenKind::FalseKeyword },\
    {"float", TokenKind::FloatKeyword },\
    {"for", TokenKind::ForKeyword },\
    {"friend", TokenKind::FriendKeyword },\
    {"goto", TokenKind::GotoKeyword },\
    {"if", TokenKind::IfKeyword },\
    {"inline", TokenKind::InlineKeyword },\
    {"int", TokenKind::IntKeyword },\
    {"long", TokenKind::LongKeyword },\
    {"mutable", TokenKind::MutableKeyword },\
    {"namespace", TokenKind::NamespaceKeyword },\
    {"new", TokenKind::NewKeyword },\
    {"noexcept", TokenKind::NoexceptKeyword },\
    {"not", TokenKind::NotKeyword },\
    {"not_eq", TokenKind::Not_eqKeyword },\
    {"nullptr", TokenKind::NullptrKeyword },\
    {"operator", TokenKind::OperatorKeyword },\
    {"or", TokenKind::OrKeyword },\
    {"or_eq", TokenKind::Or_eqKeyword },\
    {"private", TokenKind::PrivateKeyword },\
    {"protected", TokenKind::ProtectedKeyword },\
    {"public", TokenKind::PublicKeyword },\
    {"reflexpr ", TokenKind::ReflexprKeyword },\
    {"register", TokenKind::RegisterKeyword },\
    {"reinterpret_cast", TokenKind::Reinterpret_castKeyword },\
    {"requires", TokenKind::RequiresKeyword },\
    {"return", TokenKind::ReturnKeyword },\
    {"short", TokenKind::ShortKeyword },\
    {"signed", TokenKind::SignedKeyword },\
    {"sizeof", TokenKind::SizeofKeyword },\
    {"static", TokenKind::StaticKeyword },\
    {"static_assert", TokenKind::Static_assertKeyword },\
    {"static_cast", TokenKind::Static_castKeyword },\
    {"struct", TokenKind::StructKeyword },\
    {"switch", TokenKind::SwitchKeyword },\
    {"synchronized", TokenKind::SynchronizedKeyword },\
    {"template", TokenKind::TemplateKeyword },\
    {"this", TokenKind::ThisKeyword },\
    {"thread_local", TokenKind::Thread_localKeyword },\
    {"throw", TokenKind::ThrowKeyword },\
    {"true", TokenKind::TrueKeyword },\
    {"try", TokenKind::TryKeyword },\
    {"typedef", TokenKind::TypedefKeyword },\
    {"typeid", TokenKind::TypeidKeyword },\
    {"typename", TokenKind::TypenameKeyword },\
    {"union", TokenKind::UnionKeyword },\
    {"unsigned", TokenKind::UnsignedKeyword },\
    {"using", TokenKind::UsingKeyword },\
    {"virtual", TokenKind::VirtualKeyword },\
    {"void", TokenKind::VoidKeyword },\
    {"volatile", TokenKind::VolatileKeyword },\
    {"wchar_t", TokenKind::Wchar_tKeyword },\
    {"while", TokenKind::WhileKeyword },\
    {"xor", TokenKind::XorKeyword },\
    {"xor_eq", TokenKind::Xor_eqKeyword },\
    {"alignof", TokenKind::AlignofKeyword },\
    {"allocator", TokenKind::AllocatorKeyword },\
    {"array", TokenKind::ArrayKeyword },\
    {"atomic", TokenKind::AtomicKeyword },\
    {"boolalpha", TokenKind::BoolalphaKeyword },\
    {"chrono", TokenKind::ChronoKeyword },\
    {"cin", TokenKind::CinKeyword },\
    {"clearerr", TokenKind::ClearerrKeyword },\
    {"clock", TokenKind::ClockKeyword },\
    {"condition_variable", TokenKind::Condition_variableKeyword },\
    {"cout", TokenKind::CoutKeyword },\
    {"cerr", TokenKind::CerrKeyword },\
    {"clog", TokenKind::ClogKeyword },\
    {"complex", TokenKind::ComplexKeyword },\
    {"condition_variable_any", TokenKind::Condition_variable_anyKeyword },\
    {"const_cast", TokenKind::Const_castKeyword },\
    {"deque", TokenKind::DequeKeyword },\
    {"dynamic_pointer_cast", TokenKind::Dynamic_pointer_castKeyword },\
    {"enable_if", TokenKind::Enable_ifKeyword },\
    {"endl", TokenKind::EndlKeyword },\
    {"exception_ptr", TokenKind::Exception_ptrKeyword },\
    {"explicit", TokenKind::ExplicitKeyword },\
    {"false", TokenKind::FalseKeyword },\
    {"filebuf", TokenKind::FilebufKeyword },\
    {"fstream", TokenKind::FstreamKeyword },\
    {"future", TokenKind::FutureKeyword },\
    {"get_time", TokenKind::Get_timeKeyword },\
    {"has_virtual_destructor", TokenKind::Has_virtual_destructorKeyword },\
    {"hex", TokenKind::HexKeyword },\
    {"ignore", TokenKind::IgnoreKeyword },\
    {"imaxstream", TokenKind::ImaxstreamKeyword },\
    {"includes", TokenKind::IncludesKeyword },\
    {"inner_product", TokenKind::Inner_productKeyword },\
    {"ios_base", TokenKind::Ios_baseKeyword },\
    {"isalnum", TokenKind::IsalnumKeyword },\
    {"isalpha", TokenKind::IsalphaKeyword },\
    {"isdigit", TokenKind::IsdigitKeyword },\
    {"isprint", TokenKind::IsprintKeyword },\
    {"istream", TokenKind::IstreamKeyword },\
    {"iterator", TokenKind::IteratorKeyword },\
    {"kill_dependency", TokenKind::Kill_dependencyKeyword },\
    {"limites", TokenKind::LimitesKeyword },\
    {"list", TokenKind::ListKeyword },\
    {"locale", TokenKind::LocaleKeyword },\
    {"make_shared", TokenKind::Make_sharedKeyword },\
    {"map", TokenKind::MapKeyword },\
    {"match_results", TokenKind::Match_resultsKeyword },\
    {"max", TokenKind::MaxKeyword },\
    {"min", TokenKind::MinKeyword },\
    {"mismatch", TokenKind::MismatchKeyword },\
    {"move", TokenKind::MoveKeyword },\
    {"multimap", TokenKind::MultimapKeyword },\
    {"mutex", TokenKind::MutexKeyword },\
    {"new_handler", TokenKind::New_handlerKeyword },\
    {"next_permutation", TokenKind::Next_permutationKeyword },\
    {"nothrow", TokenKind::NothrowKeyword },\
    {"nth_element", TokenKind::Nth_elementKeyword },\
    {"numeric_limits", TokenKind::Numeric_limitsKeyword },\
    {"ofstream", TokenKind::OfstreamKeyword },\
    {"optional", TokenKind::OptionalKeyword },\
    {"ostringstream", TokenKind::OstringstreamKeyword },\
    {"pair", TokenKind::PairKeyword },\
    {"partial_sum", TokenKind::Partial_sumKeyword },\
    {"priority_queue", TokenKind::Priority_queueKeyword },\
    {"push_heap", TokenKind::Push_heapKeyword },\
    {"queue", TokenKind::QueueKeyword },\
    {"random_device", TokenKind::Random_deviceKeyword },\
    {"range_error", TokenKind::Range_errorKeyword },\
    {"ratio", TokenKind::RatioKeyword },\
    {"regex", TokenKind::RegexKeyword },\
    {"remove_reference", TokenKind::Remove_referenceKeyword },\
    {"replace_copy", TokenKind::Replace_copyKeyword },\
    {"result_of", TokenKind::Result_ofKeyword },\
    {"reverse_copy", TokenKind::Reverse_copyKeyword },\
    {"rotate_copy", TokenKind::Rotate_copyKeyword },\
    {"runtime_error", TokenKind::Runtime_errorKeyword },\
    {"set", TokenKind::SetKeyword },\
    {"shared_mutex ", TokenKind::Shared_mutexKeyword },\
    {"shared_ptr", TokenKind::Shared_ptrKeyword },\
    {"shuffle", TokenKind::ShuffleKeyword },\
    {"stringstream", TokenKind::StringstreamKeyword },\
    {"stack", TokenKind::StackKeyword },\
    {"static_pointer_cast", TokenKind::Static_pointer_castKeyword },\
    {"streambuf", TokenKind::StreambufKeyword },\
    {"string", TokenKind::StringKeyword },\
    {"strstream", TokenKind::StrstreamKeyword },\
    {"swap", TokenKind::SwapKeyword },\
    {"system_error", TokenKind::System_errorKeyword },\
    {"thread", TokenKind::ThreadKeyword },\
    {"time", TokenKind::TimeKeyword },\
    {"tuple", TokenKind::TupleKeyword },\
    {"type_info", TokenKind::Type_infoKeyword },\
    {"unique_ptr", TokenKind::Unique_ptrKeyword },\
    {"unordered_map", TokenKind::Unordered_mapKeyword },\
    {"unordered_set ", TokenKind::Unordered_setKeyword },\
    {"valarray", TokenKind::ValarrayKeyword },\
    {"vector", TokenKind::VectorKeyword },\
    {"void_t ", TokenKind::Void_tKeyword },\
    {"vprintf", TokenKind::VprintfKeyword }, \
    {"module", TokenKind::ModuleKeyword }, \
    {"always_ff", TokenKind::AlwaysFFKeyword }, \
    {"always_comb", TokenKind::AlwaysCombKeyword }, \
    {"initial", TokenKind::InitialKeyword }, \
    { "nettype", TokenKind::NetTypeKeyword },\
    { "soft", TokenKind::SoftKeyword }, \
    {"bit", TokenKind::BitKeyword}, \
    {"endmodule", TokenKind::EndModuleKeyword}
    
    

