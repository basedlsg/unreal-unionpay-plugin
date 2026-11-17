# UnionPay Merchant API Access Research Report

## Executive Summary
Based on comprehensive research, multiple pathways exist for UnionPay integration, with payment service providers offering the most accessible route for developers. Direct UnionPay access requires complex merchant certification processes.

## Primary Integration Options

### 1. Payment Service Provider Route (RECOMMENDED)

#### **Mastercard Gateway - UnionPay SecurePay**
- **Requirements**: Must be boarded onto UPOP through a payment service provider
- **Setup**: Merchant profile configured with UPOP account details
- **Integration**: 
  - Hosted Checkout (Version 34+): Automatic availability
  - Direct Payment: `sourceOfFunds.type = UNION_PAY`
- **Testing**: UnionPay SecurePay Emulator available
- **Compliance**: UnionPay SecurePay branding requirements

#### **PPRO - UnionPay SecurePay**
- **Coverage**: Global (excluding sanctioned countries)
- **Currencies**: AUD, CAD, CHF, CNY, EUR, GBP, HKD, JPY, NZD, SGD, USD
- **Transaction Limits**: CNY 0.01 - 300,000
- **Features**: 
  - Web/WAP implementation
  - QR code payment support
  - Multiple language support
  - 60-minute session timeout
  - Full/partial refunds (6-month validity)

### 2. Direct UnionPay Access (COMPLEX)

#### **UnionPay International Developer Portal**
- **Status**: Limited public access (JavaScript-dependent platform)
- **Process**: Submit application to Developer Operations team
- **Requirements**: 
  - Acquirer or direct-connected merchant qualification
  - Local UnionPay office contact
  - Qualification application process
  - Development and testing with UnionPay
  - Go-live approval

#### **Contact Information**
- **Hotline**: 4007795516 (China-based)
- **Developer Portal**: developer.unionpayintl.com (limited access)
- **Open Platform**: open.unionpayintl.com (JavaScript required)

## Recommended Implementation Strategy

### Phase 1: Quick Start (Weeks 1-2)
1. **Register with PPRO**: Most accessible UnionPay integration
   - Supports all major currencies
   - Comprehensive API documentation
   - Immediate sandbox access
   - Global coverage

2. **Mastercard Gateway Backup**: Secondary integration option
   - Enterprise-grade infrastructure
   - Advanced fraud protection
   - Comprehensive testing tools

### Phase 2: Direct Access (Weeks 3-8)
1. **Contact UnionPay Directly**: 
   - Call 4007795516 for developer operations
   - Submit merchant qualification application
   - Establish local UnionPay office relationship
   - Complete certification process

2. **Alternative Contacts**:
   - Merchant registration: merchant.unionpay.com/cas/register.do
   - Certification portal: cert.unionpay.com/cert-portal-en/

## Technical Specifications

### UnionPay SecurePay Features
- **Payment Flow**: Redirect to UnionPay secure page
- **Authentication**: Card details processed on UnionPay servers
- **Security**: PCI DSS compliant infrastructure
- **Mobile Support**: QR code payments via UnionPay app
- **Refunds**: Multiple partial refunds supported
- **Session Management**: 60-minute timeout

### Integration Requirements
- **Branding**: UnionPay logo and branding compliance
- **Languages**: Multi-language support available
- **Testing**: Comprehensive emulator environment
- **Webhooks**: Real-time transaction notifications
- **Reporting**: Detailed transaction analytics

## Risk Assessment

### Low Risk (PSP Route)
- **Pros**: Quick setup, proven infrastructure, comprehensive support
- **Cons**: Transaction fees, less customization

### High Risk (Direct Route)
- **Pros**: Lower fees, full customization, direct relationship
- **Cons**: Complex approval process, lengthy certification, uncertain timeline

## Next Steps

1. **Immediate Actions**:
   - Register with PPRO for UnionPay SecurePay access
   - Set up Mastercard Gateway merchant account
   - Begin sandbox integration testing

2. **Parallel Efforts**:
   - Contact UnionPay Developer Operations team
   - Initiate merchant qualification process
   - Establish relationships with local UnionPay offices

3. **Technical Preparation**:
   - Design API architecture for multiple payment providers
   - Implement provider abstraction layer
   - Prepare for PCI DSS compliance requirements

## Conclusion

The payment service provider route through PPRO offers the fastest path to UnionPay integration with minimal risk and comprehensive support. Parallel pursuit of direct UnionPay access provides long-term strategic benefits but requires significant time investment and uncertain outcomes.

**Recommended Action**: Proceed with PPRO integration for immediate development while pursuing direct UnionPay access for future optimization.