#
# Be sure to run `pod lib lint Bigger.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|

    s.name         = "Bigger"
    s.version      = "0.13.1"
    s.summary      = "贵金属项目所使用的第三方组件(非UI部分)"
    s.homepage     = "https://git.ms.netease.com/preciousmetals/Bigger"
    s.license      = { :type => 'MIT', :file => 'LICENSE'}
    s.author       = { "YangXP" => "bjyangxiaopeng1@corp.netease.com" }
    s.source       = { :git => "https://git.ms.netease.com/preciousmetals/Bigger.git", :tag => s.version.to_s}

    s.source_files = 'Bigger/**/*.{h,cc,m,mm}'
    # s.private_header_files = 'Bigger/Classes/BLogger.h'
    s.public_header_files = 'Bigger/include/*.h'
    s.platform = :ios
    s.ios.deployment_target = '8.0'
    s.dependency 'LDXLog'


end
